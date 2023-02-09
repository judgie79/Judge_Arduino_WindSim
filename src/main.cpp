#include <Arduino.h>
#include <SpeedServoControl.h>
#include <SpeedFanControl.h>
#include "CommandStream.h"
#include "WindSimCommander.h"
#include "SettingsManager.h"
#include "WindControl.h"


WindControl leftWind(WIND_LEFT);
WindControl rightWind(WIND_RIGHT);

SpeedFanControl fan1;
SpeedFanControl fan2;
SpeedServoControl servo1;
SpeedServoControl servo2;

SettingsManager settingsManager;
WindSimSettings settings;

CommandStream cmdStream(new char[10]{
                      (char)CommandKey::HELLO,
                      (char)CommandKey::ALREADY_CONNECTED,
                      (char)CommandKey::DEBUG,
                      (char)CommandKey::ERROR,
                      (char)CommandKey::RECEIVED,
                      (char)CommandKey::BYE,
                      (char)WindSimCommandKey::DATA,
                      (char)WindSimCommandKey::RESETSETTINGS,
                      (char)WindSimCommandKey::GETSETTINGS,
                      (char)WindSimCommandKey::SETSETTINGS
                      },
                  10);
/*
    COMMANDS FROM SERIAL BUS
*/
WindSimCommander commander(&settingsManager, &leftWind, &rightWind);


void onSettingsChange(WindSimSettings settings)
{
  
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  settings = settingsManager.loadSettings();
  cmdStream.setOutput(&Serial);
   commander.begin(&cmdStream);
  settingsManager.settingsChangeCallback(&onSettingsChange);

  leftWind.Begin(GPIO_NUM_17, 0, GPIO_NUM_12, 2);
  rightWind.Begin(GPIO_NUM_35, 1, GPIO_NUM_13, 3);

  LOGD_INFO("ready");
}

#define DATA_HZ 1000 / 60

long lastDataSend = 0;

void loop() {

  commander.read();

}