#ifndef __SETTINGSMANAGER_H
#define __SETTINGSMANAGER_H
#include "Arduino.h"
#include <DebugOut.h>
#if defined(ESP32)
#include "PreferencesDataStore.h"
#else
#include "EEPROMDataStore.h"
#endif
#include "WindSimSettings.h"



typedef void(*SettingsChanged) (WindSimSettings);

class SettingsManager
{
private:
#if defined(ESP32)
    PreferencesDataStore dataStore;
#else
    EEPROMDataStore dataStore;
#endif
    
    SettingsChanged settingsChanged;

    WindSimSettings currentSettings;
public:
    SettingsManager();

    WindSimSettings loadSettings();

    WindSimSettings writeSettings(WindSimSettings settings);

    WindSimSettings resetSettings();

    void settingsChangeCallback(SettingsChanged changedcallback);
};

#endif