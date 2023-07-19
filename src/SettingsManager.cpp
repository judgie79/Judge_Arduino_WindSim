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

    currentSettings = defaultSettings;

    return currentSettings;
}

WindSimSettings SettingsManager::writeSettings(WindSimSettings settings)
{

    dataStore.begin(false);
    dataStore.writeBool(settings.cockpitModeAddress, settings.cockpitMode);
    dataStore.writeBool(settings.cornerModeAddress, settings.cornerMode);

    settings.cockpitMode = dataStore.readBool(settings.cockpitModeAddress);
    settings.cornerMode = dataStore.readBool(settings.cornerModeAddress);    
    dataStore.end();

    if (this->settingsChanged != nullptr)
    {
        this->settingsChanged(settings);
    }

    currentSettings = settings;

    return currentSettings;
}

WindSimSettings SettingsManager::loadSettings()
{
    if (!currentSettings.settingsLoaded) {
        dataStore.begin(true);
        currentSettings.settingsStored = dataStore.readBool(currentSettings.settingsStoredAddress);
        dataStore.end();

        if (!currentSettings.settingsStored)
        {
            writeSettings(currentSettings);
            dataStore.begin(false);
            dataStore.writeBool(currentSettings.settingsStoredAddress, true);
            currentSettings.settingsStored = true;
            dataStore.end();
        } else {
            dataStore.begin();
            currentSettings.cockpitMode = dataStore.readBool(currentSettings.cockpitModeAddress);
            currentSettings.cornerMode = dataStore.readBool(currentSettings.cornerModeAddress);    
            dataStore.end();
        }

        if (this->settingsChanged != nullptr)
        {
            this->settingsChanged(currentSettings);
        }
    }

    return currentSettings;
}