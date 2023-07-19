#ifndef __WindSimSettings_h
#define __WindSimSettings_h

class WindSimSettings
{
    public:
        WindSimSettings()
        {
        }

        bool settingsStored = false;
        bool settingsLoaded = false;
        bool cockpitMode = false;
        bool cornerMode = false;

        const char*   settingsStoredAddress = "wind";
        const char*   cockpitModeAddress = "cockpitMode";
        const char*   cornerModeAddress = "cornerMode";
};

#endif