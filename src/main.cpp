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
  while (!Serial)
  {
    
  }
  
  delay(2000);
  cmdStream.setOutput(&Serial);
  commander.begin(&cmdStream);
  leftWind.Begin(GPIO_NUM_4, 0, GPIO_NUM_21, 2);
  rightWind.Begin(GPIO_NUM_14, 1, GPIO_NUM_32, 3);

  commander.init();
  LOGD_INFO("ready");
}


void loop() {
  commander.read();
}