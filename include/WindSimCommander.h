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
	void sendData();

protected:
	void readCmd(char key, String data);

private:
	SettingsManager* settingsManager;

	void onSetSettingsCommand(String value);

	void onGetSettingsCommand(String value);

	void onResetSettingsCommand(String value);

	void onGetDataCommand(String value);

	WindControl *leftWind;
	WindControl *rightWind;
};

#endif

