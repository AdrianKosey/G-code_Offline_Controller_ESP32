#include "gcode_parser.h"

GCodeParser::GCodeParser() { reset(); }

void GCodeParser::reset() { state = GCodeState{}; }

const GCodeState& GCodeParser::getState() const { return state; }

float GCodeParser::toMillimeters(float value) const
{
    return state.metric ? value : value * 25.4f;
}

String GCodeParser::stripComment(const String& line) const
{
    String result = line;

    int semicolon = result.indexOf(';');
    if (semicolon >= 0)
        result = result.substring(0, semicolon);

    int openParen;
    while ((openParen = result.indexOf('(')) >= 0)
    {
        int closeParen = result.indexOf(')', openParen);

        if (closeParen < 0)
        {
            result = result.substring(0, openParen);
            break;
        }

        result = result.substring(0, openParen) + result.substring(closeParen + 1);
    }

    result.trim();
    return result;
}

uint8_t GCodeParser::tokenize(const String& line, Word* words) const
{
    uint8_t count = 0;
    uint16_t i = 0;
    uint16_t len = line.length();

    while (i < len && count < MAX_WORDS_PER_LINE)
    {
        char c = line[i];

        if (isAlpha(c))
        {
            char letter = toupper(c);
            i++;
            uint16_t start = i;

            while (i < len && (isDigit(line[i]) || line[i] == '.' || line[i] == '-' || line[i] == '+'))
                i++;

            if (i > start)
            {
                words[count].letter = letter;
                words[count].value = line.substring(start, i).toFloat();
                count++;
            }
        }
        else
        {
            i++;
        }
    }

    return count;
}

bool GCodeParser::hasWord(const Word* words, uint8_t count, char letter, float& value) const
{
    for (uint8_t i = 0; i < count; i++)
    {
        if (words[i].letter == letter)
        {
            value = words[i].value;
            return true;
        }
    }

    return false;
}

GCodeCommand GCodeParser::parseLine(const String& rawLine)
{
    GCodeCommand command;

    String line = stripComment(rawLine);

    if (line.length() == 0)
    {
        command.feedRate = state.feedRate;
        command.spindle = state.spindle;
        return command;
    }

    Word words[MAX_WORDS_PER_LINE];
    uint8_t count = tokenize(line, words);

    float value;

    if (hasWord(words, count, 'G', value))
    {
        switch ((uint8_t)value)
        {
            case 0: state.lastMotion = GCodeMotionType::Rapid; break;
            case 1: state.lastMotion = GCodeMotionType::Linear; break;
            case 2: state.lastMotion = GCodeMotionType::ArcCW; break;
            case 3: state.lastMotion = GCodeMotionType::ArcCCW; break;
            case 20: state.metric = false; break;
            case 21: state.metric = true; break;
            case 90: state.absoluteMode = true; break;
            case 91: state.absoluteMode = false; break;
            default: break; // G17/G18/G19/G28, etc. - not handled in this parser
        }
    }

    if (hasWord(words, count, 'M', value))
    {
        switch ((uint8_t)value)
        {
            case 3: state.spindle = SpindleState::ClockwiseOn; break;
            case 4: state.spindle = SpindleState::CounterClockwiseOn; break;
            case 5: state.spindle = SpindleState::Off; break;
            case 2:
            case 30:
                command.isProgramEnd = true;
                break;
            default: break;
        }
    }

    if (hasWord(words, count, 'F', value))
        state.feedRate = toMillimeters(value);

    float x, y, z;
    bool hasX = hasWord(words, count, 'X', x);
    bool hasY = hasWord(words, count, 'Y', y);
    bool hasZ = hasWord(words, count, 'Z', z);

    if (hasX || hasY || hasZ)
    {
        GCodePosition target = state.position;

        if (hasX) target.x = state.absoluteMode ? toMillimeters(x) : state.position.x + toMillimeters(x);
        if (hasY) target.y = state.absoluteMode ? toMillimeters(y) : state.position.y + toMillimeters(y);
        if (hasZ) target.z = state.absoluteMode ? toMillimeters(z) : state.position.z + toMillimeters(z);

        state.position = target;

        command.hasTarget = true;
        command.target = target;
        command.motion = state.lastMotion;
    }

    command.feedRate = state.feedRate;
    command.spindle = state.spindle;

    return command;
}