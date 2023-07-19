// WindSimCommander.h

#ifndef _WindSimCommander_h
#define _WindSimCommander_h

#include "Arduino.h"
#include <Commander.h>
#include <CommandStream.h>
#include <Commands.h>
#include "WindControl.h"
#include "WindSimCommands.h"
#include "SettingsManager.h"

class WindSimCommander : public Commander
{
public:
	WindSimCommander(SettingsManager* settingsManager, WindControl *leftWind, WindControl *rightWind);
	//WindSimCommander(WindControl *leftWind, WindControl *rightWind);
	void sendData();

protected:
	void readCmd(char key, const char * data, uint8_t valueLength);

private:
	SettingsManager* settingsManager;

	void onSetSettingsCommand(const char * value, uint8_t valueLength);

	void onGetSettingsCommand(const char * value, uint8_t valueLength);

	void onResetSettingsCommand(const char * value, uint8_t valueLength);

	void onGetDataCommand(const char * value, uint8_t valueLength);

	WindControl *leftWind;
	WindControl *rightWind;
};

#endif

