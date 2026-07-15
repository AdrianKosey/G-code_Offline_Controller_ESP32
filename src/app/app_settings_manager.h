#pragma once

#include <Preferences.h>
#include "../../include/config.h"

enum class AppLanguage { Spanish, English };

class AppSettingsManager
{
public:
    AppSettingsManager();

    float getJogFeedRate() const;
    void setJogFeedRate(float value);

    float getFramingFeedRate() const;
    void setFramingFeedRate(float value);

    bool isGcodePreviewEnabled() const;
    void setGcodePreviewEnabled(bool enabled);

    bool isJobRecoveryEnabled() const;
    void setJobRecoveryEnabled(bool enabled);

    bool isFramingEnabled() const;
    void setFramingEnabled(bool enabled);

    AppLanguage getLanguage() const;
    void setLanguage(AppLanguage lang);

    float getSafeZHeight() const;
    void setSafeZHeight(float value);
    bool isScreenSleepEnabled() const;
    void setScreenSleepEnabled(bool enabled);

    uint16_t getScreenSleepMinutes() const;
    void setScreenSleepMinutes(uint16_t minutes);
    bool isBuzzerEnabled() const;
    void setBuzzerEnabled(bool enabled);

private:
    Preferences prefs;
    bool screenSleepEnabled = SCREEN_SLEEP_ENABLED_DEFAULT;
    uint16_t screenSleepMinutes = SCREEN_SLEEP_MINUTES_DEFAULT;
    float jogFeedRate = JOG_FEED_RATE;
    float framingFeedRate = FRAMING_FEED_RATE;
    bool gcodePreviewEnabled = true;
    bool jobRecoveryEnabled = false;
    float safeZHeight = SAFE_Z_HEIGHT;
    bool framingEnabled = FRAMING_ENABLED_DEFAULT;
    bool buzzerEnabled = BUZZER_ENABLED_DEFAULT;
    AppLanguage language = AppLanguage::Spanish;
};

extern AppSettingsManager* g_appSettings;