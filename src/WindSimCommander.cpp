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
	uint16_t speed = getValue(value, CommandValueSeparator, 0).toInt();
	uint16_t speedStatic = getValue(value, CommandValueSeparator, 1).toInt();
	uint16_t speedLeft = getValue(value, CommandValueSeparator, 2).toInt();
	uint16_t speedRight = getValue(value, CommandValueSeparator, 3).toInt();

	WindSimSettings settings = this->settingsManager->loadSettings();

	Serial.println("settings");
	Serial.println(settings.cockpitMode);
	Serial.println(settings.cornerMode);
	Serial.println(speedLeft);
	Serial.println(speedRight);

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
	String data = "received";

	serialCmd->writeCommand((char)WindSimCommandKey::DATA, data);
}

void WindSimCommander::onSetSettingsCommand(String value)
{
	bool cockpitMode = getValue(value, CommandValueSeparator, 0).toInt() == 1 ? true : false;
	bool cornerMode = getValue(value, CommandValueSeparator, 1).toInt() == 1 ? true : false;
	Serial.println(cockpitMode);
	Serial.println(cornerMode);

	WindSimSettings settings = this->settingsManager->loadSettings();

	settings.cockpitMode = cockpitMode;
	settings.cornerMode = cornerMode;
	
	WindSimSettings storedSettings = this->settingsManager->writeSettings(settings);
	// write back
	serialCmd->writeCommand((char)WindSimCommandKey::GETSETTINGS, String(storedSettings.cockpitMode) + String(CommandValueSeparator) + String(storedSettings.cornerMode));
}

void WindSimCommander::onGetSettingsCommand(String value)
{
	WindSimSettings settings = this->settingsManager->loadSettings();
	// write back
	serialCmd->writeCommand((char)WindSimCommandKey::GETSETTINGS, String(settings.cockpitMode) + String(CommandValueSeparator) + String(settings.cornerMode));
}

void WindSimCommander::onResetSettingsCommand(String value)
{
	// reset settings
	WindSimSettings storedSettings = this->settingsManager->resetSettings();
	// write back
	serialCmd->writeCommand(CommandKey::RECEIVED, CommandToken);
}

