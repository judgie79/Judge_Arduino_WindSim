#include "WindSimCommander.h"


WindSimCommander::WindSimCommander(SettingsManager* settingsManager, WindControl *leftWind, WindControl *rightWind) : Commander()
// WindSimCommander::WindSimCommander(WindControl *leftWind, WindControl *rightWind) : Commander()
{
	this->settingsManager = settingsManager;
	this->leftWind = leftWind;
	this->rightWind = rightWind;
}

void WindSimCommander::readCmd(char key, const char * data, uint8_t valueLength)
{        
	switch (key)
	{
	case (char)WindSimCommandKey::DATA:
		if (isConnected())
		{
			onGetDataCommand(data, valueLength);
		}
		break;

	case (char)WindSimCommandKey::SETSETTINGS:
		if (isConnected())
		{
			onSetSettingsCommand(data, valueLength);
		}
		break;

	case (char)WindSimCommandKey::GETSETTINGS:
		if (isConnected())
		{
			onGetSettingsCommand(data, valueLength);
		}
		break;


	case (char)WindSimCommandKey::RESETSETTINGS:
		if (isConnected())
		{
			onResetSettingsCommand(data, valueLength);
		}
		break;
	default:
		Commander::readCmd(key, data, valueLength);
		break;
	}
}

void WindSimCommander::onGetDataCommand(const char * value, uint8_t valueLength)
{
	// d#0,1,2,3
	// running
	// speed
	// cornerspeed L
	// cornerspeed R
	uint8_t speed = getValue(value, valueLength, CommandValueSeparator, 0).toInt();
	uint8_t speedStatic = getValue(value, valueLength, CommandValueSeparator, 1).toInt();
	uint8_t speedLeft = getValue(value, valueLength, CommandValueSeparator, 2).toInt();
	uint8_t speedRight = getValue(value, valueLength, CommandValueSeparator, 3).toInt();

	WindSimSettings settings = this->settingsManager->loadSettings();

	if (settings.cockpitMode)
	{
		leftWind->setSpeed(speedStatic);
		rightWind->setSpeed(speedStatic);
	} else if (settings.cornerMode)
	{
		leftWind->setSpeed(speedLeft);
		rightWind->setSpeed(speedRight);
	} else 
	{
		leftWind->setSpeed(speed);
		rightWind->setSpeed(speed);
	}
	sendData();
}

void WindSimCommander::sendData()
{
	serialCmd->writeCommand((char)WindSimCommandKey::DATA, "received");
}

void WindSimCommander::onSetSettingsCommand(const char * value, uint8_t valueLength)
{
	WindSimSettings settings = this->settingsManager->loadSettings();

	settings.cockpitMode = getValue(value, valueLength, CommandValueSeparator, 0).toInt() == 1 ? true : false;
	settings.cornerMode = getValue(value, valueLength, CommandValueSeparator, 1).toInt() == 1 ? true : false;
	
	WindSimSettings storedSettings = this->settingsManager->writeSettings(settings);
}

void WindSimCommander::onGetSettingsCommand(const char * value, uint8_t valueLength)
{
	WindSimSettings settings = this->settingsManager->loadSettings();
	// write back
	char data[3] = "";
	data[0] = settings.cockpitMode ? '1' : '0';
	data[1] = (char)CommandValueSeparator;
	data[2] = settings.cornerMode ? '1' : '0';

	serialCmd->writeCommand((char)WindSimCommandKey::GETSETTINGS, data);
}

void WindSimCommander::onResetSettingsCommand(const char * value, uint8_t valueLength)
{
	// reset settings
	WindSimSettings storedSettings = this->settingsManager->resetSettings();
	// write back
	serialCmd->writeCommand(CommandKey::RECEIVED, CommandToken.c_str());
}