#include "tool_profile_manager.h"

ToolProfileManager::ToolProfileManager()
{
    prefs.begin("tools", false);
    load();
    activeIndex = prefs.getChar("active", -1);
}

const std::vector<ToolProfile>& ToolProfileManager::getProfiles() const { return profiles; }

void ToolProfileManager::load()
{
    profiles.clear();
    uint8_t count = prefs.getUChar("count", 0);

    for (uint8_t i = 0; i < count && i < MAX_PROFILES; i++)
    {
        String prefix = "t" + String(i) + "_";

        ToolProfile p;
        p.name = prefs.getString((prefix + "n").c_str(), "Herramienta " + String(i + 1));
        p.type = (ToolType)prefs.getUChar((prefix + "t").c_str(), 0);
        p.diameterMm = prefs.getFloat((prefix + "d").c_str(), 3.0f);
        p.defaultPower = prefs.getFloat((prefix + "p").c_str(), 50.0f);
        p.usageSeconds = prefs.getUInt((prefix + "u").c_str(), 0);

        profiles.push_back(p);
    }
}

void ToolProfileManager::saveAll()
{
    prefs.putUChar("count", (uint8_t)profiles.size());

    for (uint8_t i = 0; i < profiles.size(); i++)
    {
        String prefix = "t" + String(i) + "_";
        const ToolProfile& p = profiles[i];

        prefs.putString((prefix + "n").c_str(), p.name);
        prefs.putUChar((prefix + "t").c_str(), (uint8_t)p.type);
        prefs.putFloat((prefix + "d").c_str(), p.diameterMm);
        prefs.putFloat((prefix + "p").c_str(), p.defaultPower);
        prefs.putUInt((prefix + "u").c_str(), p.usageSeconds);
    }
}

void ToolProfileManager::addProfile(const ToolProfile& profile)
{
    if (profiles.size() >= MAX_PROFILES)
        return;

    profiles.push_back(profile);
    saveAll();
}

void ToolProfileManager::updateProfile(uint8_t index, const ToolProfile& profile)
{
    if (index >= profiles.size())
        return;

    profiles[index] = profile;
    saveAll();
}

void ToolProfileManager::deleteProfile(uint8_t index)
{
    if (index >= profiles.size())
        return;

    profiles.erase(profiles.begin() + index);
    saveAll();
}

void ToolProfileManager::addUsageSeconds(uint8_t index, uint32_t seconds)
{
    if (index >= profiles.size())
        return;

    profiles[index].usageSeconds += seconds;

    String key = "t" + String(index) + "_u";
    prefs.putUInt(key.c_str(), profiles[index].usageSeconds);
}


int8_t ToolProfileManager::getActiveIndex() const { return activeIndex; }

void ToolProfileManager::setActiveIndex(int8_t index)
{
    activeIndex = index;
    prefs.putChar("active", index);
}