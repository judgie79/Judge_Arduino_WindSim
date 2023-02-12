#include "WindSimCommander.h"


WindSimCommander::WindSimCommander(SettingsManager* settingsManager, WindControl *leftWind, WindControl *rightWind) : Commander()
{
	this->settingsManager = settingsManager;
	this->leftWind = leftWind;
	this->rightWind = rightWind;
}

void WindSimCommander::readCmd(char key, String data)
{        
	switch (key)
	{
	case (char)WindSimCommandKey::DATA:
		if (isConnected())
		{
			onGetDataCommand(data);
		}
		break;

	case (char)WindSimCommandKey::SETSETTINGS:
		if (isConnected())
		{
			onSetSettingsCommand(data);
		}
		break;

	case (char)WindSimCommandKey::GETSETTINGS:
		if (isConnected())
		{
			onGetSettingsCommand(data);
		}
		break;


	case (char)WindSimCommandKey::RESETSETTINGS:
		if (isConnected())
		{
			onResetSettingsCommand(data);
		}
		break;
	default:
		Commander::readCmd(key, data);
		break;
	}
}

void WindSimCommander::onGetDataCommand(String value)
{
	// running
	// speed
	// cornerspeed
	// 
	uint8_t speed = getValue(value, CommandValueSeparator, 0).toInt();
	uint8_t speedStatic = getValue(value, CommandValueSeparator, 1).toInt();
	uint8_t speedLeft = getValue(value, CommandValueSeparator, 2).toInt();
	uint8_t speedRight = getValue(value, CommandValueSeparator, 3).toInt();

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

void WindSimCommander::onSetSettingsCommand(String value)
{
	WindSimSettings settings = this->settingsManager->loadSettings();

	settings.cockpitMode = getValue(value, CommandValueSeparator, 0).toInt() == 1 ? true : false;
	settings.cornerMode = getValue(value, CommandValueSeparator, 1).toInt() == 1 ? true : false;
	
	WindSimSettings storedSettings = this->settingsManager->writeSettings(settings);
	// write back
	serialCmd->writeCommand((char)WindSimCommandKey::GETSETTINGS, getSettingsData(&settings));
}

void WindSimCommander::onGetSettingsCommand(String value)
{
	WindSimSettings settings = this->settingsManager->loadSettings();
	// write back
	serialCmd->writeCommand((char)WindSimCommandKey::GETSETTINGS, getSettingsData(&settings));
}

void WindSimCommander::onResetSettingsCommand(String value)
{
	// reset settings
	WindSimSettings storedSettings = this->settingsManager->resetSettings();
	// write back
	serialCmd->writeCommand(CommandKey::RECEIVED, CommandToken);
}

const char* getSettingsData(WindSimSettings *settings)
{
	char data[3] = "";
	data[0] = settings->cockpitMode;
	data[1] = CommandValueSeparator;
	data[2] = settings->cornerMode;
	return data;
}