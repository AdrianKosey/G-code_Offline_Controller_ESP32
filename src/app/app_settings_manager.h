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

private:
    Preferences prefs;

    float jogFeedRate = JOG_FEED_RATE;
    float framingFeedRate = FRAMING_FEED_RATE;
    bool gcodePreviewEnabled = true;
    bool jobRecoveryEnabled = false;
    float safeZHeight = SAFE_Z_HEIGHT;
    bool framingEnabled = FRAMING_ENABLED_DEFAULT;
    AppLanguage language = AppLanguage::Spanish;
};

extern AppSettingsManager* g_appSettings;