#include "WindSimCommander.h"


WindSimCommander::WindSimCommander(SettingsManager* settingsManager, WindControl *leftWind, WindControl *rightWind) : Commander()
// WindSimCommander::WindSimCommander(WindControl *leftWind, WindControl *rightWind) : Commander()
{
	this->settingsManager = settingsManager;
	this->leftWind = leftWind;
	this->rightWind = rightWind;
}

void WindSimCommander::init() {

	WindSimSettings storedSettings = this->settingsManager->loadSettings();
	leftWind->setSettings(storedSettings.fanMaxKmh, storedSettings.fanPercentage, storedSettings.valveMaxOpenSpeed);
	rightWind->setSettings(storedSettings.fanMaxKmh, storedSettings.fanPercentage, storedSettings.valveMaxOpenSpeed);

	leftWind->setSpeed(0);
	rightWind->setSpeed(0);
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

	// case (char)CommandKey::HELLO:
	// 	Commander::readCmd(key, data, valueLength);
	// 	break;
	default:
		Commander::readCmd(key, data, valueLength);
		break;
	}
}

void WindSimCommander::onHelloCommand(const char * value) {
	Commander::onHelloCommand(value);

}

void WindSimCommander::onGetDataCommand(const char * value, uint8_t valueLength)
{
	// d#0,1,2,3
	// running
	// speed
	// cornerspeed L
	// cornerspeed R
	uint16_t speed = getValue(value, valueLength, CommandValueSeparator, 0).toInt();
	uint16_t speedStatic = getValue(value, valueLength, CommandValueSeparator, 1).toInt();
	uint16_t speedLeft = getValue(value, valueLength, CommandValueSeparator, 2).toInt();
	uint16_t speedRight = getValue(value, valueLength, CommandValueSeparator, 3).toInt();
	bool gameIsRunning = getValue(value, valueLength, CommandValueSeparator, 4).toInt() == 1;

	WindSimSettings settings = this->settingsManager->loadSettings();

	if (!gameIsRunning) {
		leftWind->setSpeed(0);
		rightWind->setSpeed(0);
	} else {
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

	settings.valveMaxOpenSpeed = getValue(value, valueLength, CommandValueSeparator, 2).toInt();
	settings.fanMaxKmh = getValue(value, valueLength, CommandValueSeparator, 3).toInt();
	settings.fanPercentage = getValue(value, valueLength, CommandValueSeparator, 4).toInt();
	
	WindSimSettings storedSettings = this->settingsManager->writeSettings(settings);

	leftWind->setSettings(storedSettings.fanMaxKmh, storedSettings.fanPercentage, storedSettings.valveMaxOpenSpeed);
	rightWind->setSettings(storedSettings.fanMaxKmh, storedSettings.fanPercentage, storedSettings.valveMaxOpenSpeed);
}

void WindSimCommander::onGetSettingsCommand(const char * value, uint8_t valueLength)
{
	WindSimSettings settings = this->settingsManager->loadSettings();

	String t1 = String(settings.valveMaxOpenSpeed, 10);
	String t2 = String(settings.fanMaxKmh, 10);
	String t3 = String(settings.fanPercentage, 10);
	
	String temp = (settings.cockpitMode ? "1" : "0") + String((char)CommandValueSeparator) + 
		(settings.cornerMode ? "1" : "0") + String((char)CommandValueSeparator) + 
		t1 + 
		String((char)CommandValueSeparator) + t2 + 
		String((char)CommandValueSeparator) + t3;

	serialCmd->writeCommand((char)WindSimCommandKey::GETSETTINGS, temp.c_str());
}

void WindSimCommander::onResetSettingsCommand(const char * value, uint8_t valueLength)
{
	// reset settings
	WindSimSettings storedSettings = this->settingsManager->resetSettings();
	// write back
	serialCmd->writeCommand(CommandKey::RECEIVED, CommandToken.c_str());

	
	leftWind->setSettings(storedSettings.fanMaxKmh, storedSettings.fanPercentage, storedSettings.valveMaxOpenSpeed);
	rightWind->setSettings(storedSettings.fanMaxKmh, storedSettings.fanPercentage, storedSettings.valveMaxOpenSpeed);
}