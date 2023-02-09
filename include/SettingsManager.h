#ifndef __SETTINGSMANAGER_H
#define __SETTINGSMANAGER_H
#include "Arduino.h"
#include <DebugOut.h>
#include <Preferences.h>
#include "WindSimSettings.h"



typedef void(*SettingsChanged) (WindSimSettings);

class SettingsManager
{
private:
    /* data */
    Preferences dataStore;
    SettingsChanged settingsChanged;
public:
    SettingsManager();

    WindSimSettings loadSettings();

    WindSimSettings writeSettings(WindSimSettings settings);

    WindSimSettings resetSettings();

    void settingsChangeCallback(SettingsChanged changedcallback);
};

#endif