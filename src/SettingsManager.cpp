#include "SettingsManager.h"

SettingsManager::SettingsManager()
: dataStore()
{
}

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
        dataStore.begin("windsim",false);
    dataStore.putBool(settings.cockpitModeAddress, settings.cockpitMode);
    dataStore.putBool(settings.cornerModeAddress, settings.cornerMode);

    settings.cockpitMode = dataStore.getBool(settings.cockpitModeAddress);
    settings.cornerMode = dataStore.getBool(settings.cornerModeAddress);    
    dataStore.end();
        Serial.println(settings.cockpitMode);
        Serial.println(settings.cornerMode);

     if (this->settingsChanged != nullptr)
     {
         this->settingsChanged(settings);
     }

    return settings;
}

WindSimSettings SettingsManager::loadSettings()
{
    
    WindSimSettings settings;
    dataStore.begin("windsim",true);
    settings.settingsStored = dataStore.getBool(settings.settingsStoredAddress);
dataStore.end();
    if (!settings.settingsStored)
    {
        writeSettings(settings);
        dataStore.begin("windsim",false);
        dataStore.putBool(settings.settingsStoredAddress, true);
        settings.settingsStored = true;
        dataStore.end();
    } else {
        dataStore.begin("windsim", true);
        settings.cockpitMode = dataStore.getBool(settings.cockpitModeAddress);
        settings.cornerMode = dataStore.getBool(settings.cornerModeAddress);    
        dataStore.end();
    }
    if (this->settingsChanged != nullptr)
    {
        this->settingsChanged(settings);
    }
    
    return settings;
}