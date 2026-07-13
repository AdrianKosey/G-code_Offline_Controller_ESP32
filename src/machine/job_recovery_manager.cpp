#include "job_recovery_manager.h"

void JobRecoveryManager::begin()
{
    if (!SD.exists(RECOVERY_PATH))
        return;

    File file = SD.open(RECOVERY_PATH, FILE_READ);

    if (!file)
        return;

    if (file.size() == sizeof(RecoverySnapshot))
        file.read((uint8_t*)&snapshot, sizeof(RecoverySnapshot));
    else
        snapshot.valid = false; // Corrupt file/incorrect size - ignored, no recovery attempt

    file.close();
}

void JobRecoveryManager::startJob(const String& path, uint32_t totalLines)
{
    snapshot = RecoverySnapshot{};
    snapshot.valid = true;
    path.toCharArray(snapshot.path, sizeof(snapshot.path));
    snapshot.totalLines = totalLines;

    lastSavedLine = 0;
    lastSaveAt = millis();

    persist();
}

void JobRecoveryManager::updateProgress(uint32_t line, const GCodeState& state, uint8_t spindleStateValue)
{
    if (!snapshot.valid)
        return;

    bool byLine = (line - lastSavedLine) >= SAVE_LINE_INTERVAL;
    bool byTime = (millis() - lastSaveAt) >= SAVE_INTERVAL_MS;

    if (!byLine && !byTime)
        return;

    snapshot.line = line;
    snapshot.x = state.position.x;
    snapshot.y = state.position.y;
    snapshot.z = state.position.z;
    snapshot.absoluteMode = state.absoluteMode;
    snapshot.metric = state.metric;
    snapshot.feedRate = state.feedRate;
    snapshot.spindleSpeed = state.spindleSpeed;
    snapshot.spindleState = spindleStateValue;
    snapshot.plane = state.plane;
    snapshot.coordSystem = state.coordSystem;
    snapshot.toolNumber = state.toolNumber;

    persist();

    lastSavedLine = line;
    lastSaveAt = millis();
}

void JobRecoveryManager::persist()
{
    // The file is recreated on each save - simple and reliable,
    // wear and tear is absorbed by the SD card's own wear-leveling, 
    // distributed across its entire capacity.
    SD.remove(RECOVERY_PATH);

    File file = SD.open(RECOVERY_PATH, FILE_WRITE);

    if (!file)
        return;

    file.write((const uint8_t*)&snapshot, sizeof(RecoverySnapshot));
    file.close();
}

void JobRecoveryManager::clear()
{
    snapshot = RecoverySnapshot{};
    SD.remove(RECOVERY_PATH);
}

bool JobRecoveryManager::hasPendingRecovery() const { return snapshot.valid; }
const RecoverySnapshot& JobRecoveryManager::getSnapshot() const { return snapshot; }