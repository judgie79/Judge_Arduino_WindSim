#include "WindControl.h"

WindControl::WindControl(uint8_t side)
    : fan(), servo()
{
    this->side = side;
}

void WindControl::Begin(uint8_t fanPin, uint16_t fanChannel, uint16_t servoPin, uint16_t servoChannel)
{
    fan.Begin(fanPin, fanChannel);
    fan.SetFanSpeed(0);

    servo.Begin(servoPin, servoChannel);
    servo.SetPosition(0);
}

WindControl::~WindControl()
{
    servo.~SpeedServoControl();
    fan.~SpeedFanControl();
}

void WindControl::openValve(uint16_t degree)
{
    if (this-side == WIND_LEFT) {
        servo.SetPosition(degree);
    } else {
        servo.SetPosition(360 - degree);
    }
}

void WindControl::setFanSpeed(uint16_t speed)
{
    fan.SetFanSpeed(speed);
}

void WindControl::setSpeed(uint16_t speed)
{
    setFanSpeed(speed);
    openValve(map(speed, 0, 255, 0, 90));
}