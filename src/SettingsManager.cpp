#include "SettingsManager.h"

#if defined(ESP32)
SettingsManager::SettingsManager()
: dataStore("wind")
{
}
#else
    SettingsManager::SettingsManager()
: dataStore("wind")
{
}
#endif



void SettingsManager::settingsChangeCallback(SettingsChanged settingsChanged)
  {
    this->settingsChanged = settingsChanged;
  }


WindSimSettings SettingsManager::resetSettings()
{
    WindSimSettings defaultSettings;
    this->writeSettings(defaultSettings);

    if (this->settingsChanged != nullptr)
    {
        this->settingsChanged(defaultSettings);
    }

    return defaultSettings;
}

WindSimSettings SettingsManager::writeSettings(WindSimSettings settings)
{

    dataStore.begin(false);
    dataStore.write(settings.cockpitModeAddress, settings.cockpitMode);
    dataStore.write(settings.cornerModeAddress, settings.cornerMode);

    settings.cockpitMode = dataStore.readBool(settings.cockpitModeAddress);
    settings.cornerMode = dataStore.readBool(settings.cornerModeAddress);    
    dataStore.end();

    if (this->settingsChanged != nullptr)
    {
        this->settingsChanged(settings);
    }

    return settings;
}

WindSimSettings SettingsManager::loadSettings()
{
    
    WindSimSettings settings;
    dataStore.begin();
    settings.settingsStored = dataStore.readBool(settings.settingsStoredAddress);
    dataStore.end();

    if (!settings.settingsStored)
    {
        writeSettings(settings);
        dataStore.begin(false);
        dataStore.write(settings.settingsStoredAddress, true);
        settings.settingsStored = true;
        dataStore.end();
    } else {
        dataStore.begin();
        settings.cockpitMode = dataStore.readBool(settings.cockpitModeAddress);
        settings.cornerMode = dataStore.readBool(settings.cornerModeAddress);    
        dataStore.end();
    }
    if (this->settingsChanged != nullptr)
    {
        this->settingsChanged(settings);
    }
    
    return settings;
}