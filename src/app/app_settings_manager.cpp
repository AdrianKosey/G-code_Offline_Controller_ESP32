#include "app_settings_manager.h"

AppSettingsManager* g_appSettings = nullptr;

AppSettingsManager::AppSettingsManager()
{
    bool opened = prefs.begin("app", false);
    Serial.printf("[Settings] prefs.begin() = %s\n", opened ? "OK" : "FALLO");

    jogFeedRate = prefs.getFloat("jogFeed", JOG_FEED_RATE);
    framingFeedRate = prefs.getFloat("framFeed", FRAMING_FEED_RATE);
    gcodePreviewEnabled = prefs.getBool("preview", true);
    jobRecoveryEnabled = prefs.getBool("jobRecov", false);
    language = (AppLanguage)prefs.getUChar("lang", (uint8_t)AppLanguage::Spanish);

    Serial.printf("[Settings] Cargado: jogFeed=%.1f framFeed=%.1f preview=%d lang=%d\n",
        jogFeedRate, framingFeedRate, gcodePreviewEnabled, (int)language);

    g_appSettings = this;
}

float AppSettingsManager::getJogFeedRate() const { return jogFeedRate; }

void AppSettingsManager::setJogFeedRate(float value)
{
    jogFeedRate = value;
    bool ok = prefs.putFloat("jogFeed", value);
    Serial.printf("[Settings] putFloat(jogFeed, %.1f) = %s\n", value, ok ? "OK" : "FALLO");
}

float AppSettingsManager::getFramingFeedRate() const { return framingFeedRate; }

void AppSettingsManager::setFramingFeedRate(float value)
{
    framingFeedRate = value;
    prefs.putFloat("framFeed", value);
}

bool AppSettingsManager::isGcodePreviewEnabled() const { return gcodePreviewEnabled; }

void AppSettingsManager::setGcodePreviewEnabled(bool enabled)
{
    gcodePreviewEnabled = enabled;
    prefs.putBool("preview", enabled);
}

bool AppSettingsManager::isJobRecoveryEnabled() const { return jobRecoveryEnabled; }

void AppSettingsManager::setJobRecoveryEnabled(bool enabled)
{
    jobRecoveryEnabled = enabled;
    prefs.putBool("jobRecov", enabled);
}

AppLanguage AppSettingsManager::getLanguage() const { return language; }

void AppSettingsManager::setLanguage(AppLanguage lang)
{
    language = lang;
    prefs.putUChar("lang", (uint8_t)lang);
}