#pragma once

#include <Preferences.h>
#include <vector>
#include "tool_profile.h"

class ToolProfileManager
{
public:
    static constexpr uint8_t MAX_PROFILES = 10;

    ToolProfileManager();

    const std::vector<ToolProfile>& getProfiles() const;

    void addProfile(const ToolProfile& profile);
    void updateProfile(uint8_t index, const ToolProfile& profile);
    void deleteProfile(uint8_t index);

    void addUsageSeconds(uint8_t index, uint32_t seconds);

    int8_t getActiveIndex() const;
    void setActiveIndex(int8_t index);

    ToolProfile getActiveTool() const;

private:
    Preferences prefs;
    std::vector<ToolProfile> profiles;
    int8_t activeIndex = -1;
    void load();
    void saveAll();
};