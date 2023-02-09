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
public:
    WindControl(uint8_t side);
    void Begin(uint8_t fanPin, uint16_t fanChannel, uint16_t servoPin, uint16_t servoChannel);
    ~WindControl();

    void openValve(uint16_t degree);
    void setFanSpeed(uint16_t speed);

    void setSpeed(uint16_t speed);
};


#endif