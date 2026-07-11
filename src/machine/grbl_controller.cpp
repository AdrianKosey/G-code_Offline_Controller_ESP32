#include "grbl_controller.h"

void GrblController::begin(HardwareSerial& serialPort, unsigned long baud, int8_t rxPin, int8_t txPin)
{
    serial = &serialPort;
    serial->begin(baud, SERIAL_8N1, rxPin, txPin);
}

void GrblController::beginSimulated()
{
    simulated = true;
    status.state = GrblState::Idle;
}

void GrblController::sendLine(const String& line)
{
    okFlag = false;
    errorFlag = false;

    if (simulated)
    {
        GCodeCommand command = simParser.parseLine(line);

        if (command.hasTarget)
        {
            status.x = command.target.x;
            status.y = command.target.y;
            status.z = command.target.z;
        }

        status.feedRate = command.feedRate; 
        status.spindleSpeed = command.spindleSpeed;

        status.state = GrblState::Run;
        simOkAt = millis() + SIM_MOVE_DELAY_MS; 
        return;
    }

    if (!serial) return;
    serial->print(line);
    serial->print('\n');
}

void GrblController::requestStatus() { if (simulated) return; if (serial) serial->write('?'); }
void GrblController::feedHold()      { if (simulated) { status.state = GrblState::Hold; return; } if (serial) serial->write('!'); }
void GrblController::cycleStart()    { if (simulated) { status.state = GrblState::Run; return; } if (serial) serial->write('~'); }
void GrblController::softReset()     { if (simulated) { status.state = GrblState::Idle; simOkAt = 0; return; } if (serial) serial->write(0x18); }

bool GrblController::hasPendingOk() const { return okFlag || errorFlag; }

bool GrblController::consumeOk()
{
    bool wasOk = okFlag;
    okFlag = false;
    errorFlag = false;
    return wasOk;
}

const GrblStatus& GrblController::getStatus() const { return status; }

void GrblController::update()
{
    if (simulated)
    {
        if (simOkAt != 0 && millis() >= simOkAt)
        {
            okFlag = true;
            simOkAt = 0;

            if (status.state == GrblState::Run)
                status.state = GrblState::Idle;
        }
        return;
    }

    if (!serial) return;

    while (serial->available())
    {
        char c = serial->read();

        if (c == '\n' || c == '\r')
        {
            if (lineBuffer.length() > 0)
            {
                processLine(lineBuffer);
                lineBuffer = "";
            }
        }
        else
        {
            lineBuffer += c;
            if (lineBuffer.length() > 128) lineBuffer = "";
        }
    }
}

void GrblController::processLine(const String& line)
{
    if (line.startsWith("<") && line.endsWith(">"))
    {
        parseStatusReport(line);
        return;
    }

    if (line == "ok") { okFlag = true; return; }
    if (line.startsWith("error:")) { errorFlag = true; return; }
    if (line.startsWith("ALARM:")) { status.state = GrblState::Alarm; return; }

    // Mensage [MSG:...], $$ - no implement yet
}

void GrblController::parseStatusReport(const String& line)
{
    // <Idle|WPos:0.000,0.000,0.000|FS:0,0>
    int pipe1 = line.indexOf('|');
    String stateStr = (pipe1 > 0) ? line.substring(1, pipe1) : line.substring(1, line.length() - 1);

    if (stateStr == "Idle") status.state = GrblState::Idle;
    else if (stateStr == "Run") status.state = GrblState::Run;
    else if (stateStr.startsWith("Hold")) status.state = GrblState::Hold;
    else if (stateStr == "Alarm") status.state = GrblState::Alarm;
    else if (stateStr.startsWith("Door")) status.state = GrblState::Door;
    else if (stateStr == "Check") status.state = GrblState::Check;
    else if (stateStr == "Home") status.state = GrblState::Home;
    else if (stateStr == "Sleep") status.state = GrblState::Sleep;

    int posIndex = line.indexOf("WPos:");
    if (posIndex < 0) posIndex = line.indexOf("MPos:");

    if (posIndex >= 0)
    {
        int start = posIndex + 5;
        int end = line.indexOf('|', start);
        if (end < 0) end = line.indexOf('>', start);

        String posStr = line.substring(start, end);
        int c1 = posStr.indexOf(',');
        int c2 = posStr.indexOf(',', c1 + 1);

        status.x = posStr.substring(0, c1).toFloat();
        status.y = posStr.substring(c1 + 1, c2).toFloat();
        status.z = posStr.substring(c2 + 1).toFloat();
    }

    int fsIndex = line.indexOf("FS:");

    if (fsIndex >= 0)
    {
        int start = fsIndex + 3;
        int comma = line.indexOf(',', start);
        int end = line.indexOf('|', comma);
        if (end < 0) end = line.indexOf('>', comma);

        status.feedRate = line.substring(start, comma).toFloat();
        status.spindleSpeed = line.substring(comma + 1, end).toFloat();
    }
}

void GrblController::jog(char axis, float distance, float feedRate)
{
    String cmd = "$J=G91G21";
    cmd += axis;
    cmd += String(distance, 3);
    cmd += "F";
    cmd += String(feedRate, 0);

    if (simulated)
    {
        if (axis == 'X') status.x += distance;
        else if (axis == 'Y') status.y += distance;
        else if (axis == 'Z') status.z += distance;

        status.state = GrblState::Run;
        simOkAt = millis() + SIM_MOVE_DELAY_MS;
        okFlag = false;
        return;
    }

    sendLine(cmd);
}

void GrblController::jogCancel()
{
    if (simulated) { status.state = GrblState::Idle; return; }
    if (serial) serial->write(0x85);
}

void GrblController::home()
{
    if (simulated)
    {
        status.x = 0; status.y = 0; status.z = 0;
        status.state = GrblState::Idle;
        okFlag = true;
        return;
    }

    sendLine("$H");
}

void GrblController::setWorkZero()
{
    if (simulated)
    {
        status.x = 0; status.y = 0; status.z = 0;
        okFlag = true;
        return;
    }

    sendLine("G10 L20 P1 X0 Y0 Z0");
}

void GrblController::setSpindlePower(float percent, bool clockwise)
{
    percent = constrain(percent, 0.0f, 100.0f);
    float value = (percent / 100.0f) * MAX_SPINDLE_SPEED;

    if (simulated)
    {
        status.spindleSpeed = value;
        okFlag = true;
        return;
    }

    if (percent <= 0.0f)
    {
        sendLine("M5");
    }
    else
    {
        String cmd = clockwise ? "M3 S" : "M4 S";
        cmd += String((int)value);
        sendLine(cmd);
    }
}

void GrblController::setWorkZeroAxis(char axis)
{
    String cmd = "G10 L20 P1 ";
    cmd += axis;
    cmd += "0";

    if (simulated)
    {
        if (axis == 'X') status.x = 0;
        else if (axis == 'Y') status.y = 0;
        else if (axis == 'Z') status.z = 0;

        okFlag = true;
        return;
    }

    sendLine(cmd);
}

void GrblController::probeZ(float maxDistance, float feedRate)
{
    String cmd = "G38.2 Z-" + String(maxDistance, 1) + " F" + String(feedRate, 0);

    if (simulated)
    {
        status.z = 0;
        status.state = GrblState::Run;
        simOkAt = millis() + SIM_MOVE_DELAY_MS;
        return;
    }

    sendLine(cmd);
}