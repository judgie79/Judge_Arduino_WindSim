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
        
        u_int16_t valveMaxOpenSpeed = 150;
        u_int16_t fanMaxKmh = 250;
        u_int16_t fanPercentage = 100;

        const char*   settingsStoredAddress = "wind";
        const char*   cockpitModeAddress = "cockpitMode";
        const char*   cornerModeAddress = "cornerMode";

        const char*   valveMaxOpenSpeedAddress = "vMaxSpeed";

        const char*   fanMaxKmhAddress = "fMaxSpeed";

        const char*   fanPercentageAddress = "fPercentage";

};

#endif