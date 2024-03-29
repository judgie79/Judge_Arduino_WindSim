#ifndef __WindControl_H
#define __WindControl_H

#include "Arduino.h"
#include <SpeedFanControl.h>
#include <SpeedServoControl.h>

#define WIND_LEFT 0
#define WIND_RIGHT 1

class WindControl
{
private:
    SpeedFanControl fan;
    SpeedServoControl servo;
    uint8_t side;
    uint8_t maxValveSpeed;
    uint8_t maxFanKmh;
    uint8_t fanPercentage;
public:
    WindControl(uint8_t side);
    #if defined(ESP32)
    void Begin(uint8_t fanPin, uint16_t fanChannel, uint16_t servoPin, uint16_t servoChannel);
    #else
    void Begin(uint8_t fanPin, uint16_t servoPin);
    #endif
    ~WindControl();

    void openValve(uint16_t speed);
    void setFanSpeed(uint16_t speed);

    void setSpeed(uint16_t speed);

    void setMaxFanKmh(uint16_t maxKmh);
    void setMaxOpenValve(uint16_t maxSpeed);
    void setFanPercentage(uint16_t percentage);

    void setSettings(uint16_t maxFanKmh, uint16_t fanPercentage, uint16_t valveMaxOpenSpeed);
};


#endif