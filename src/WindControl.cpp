#include "WindControl.h"

WindControl::WindControl(uint8_t side)
    : fan(), servo()
{
    this->side = side;
}

#if defined(ESP32)
void WindControl::Begin(uint8_t fanPin, uint16_t fanChannel, uint16_t servoPin, uint16_t servoChannel)
{
    fan.Begin(fanPin, fanChannel);
    servo.Begin(servoPin, servoChannel);
   // fan.SetFanSpeed(0);
   // servo.SetPosition(0);
}
#else
void Begin(uint8_t fanPin, uint16_t servoPin)
{
    fan.Begin(fanPin);
    servo.Begin(servoPin);
    fan.SetFanSpeed(0);
    servo.SetPosition(0);
}
#endif



WindControl::~WindControl()
{
    servo.~SpeedServoControl();
    fan.~SpeedFanControl();
}

void WindControl::openValve(uint16_t speed)
{
    int t_speed = speed;
    if (t_speed > maxValveSpeed) {
        t_speed = maxValveSpeed;
    }

    int degree = map(t_speed, 0, maxValveSpeed, 0, 90);
    // Serial.println("*********");
    // Serial.println("speed " + String(speed, 10));
    // Serial.println("t_speed " + String(t_speed, 10));
    // Serial.println("maxValveSpeed " + String(maxValveSpeed, 10));
    // Serial.println("degree " + String(degree, 10));
     //Serial.println("*********");


    if (this->side == WIND_LEFT) {
       // Serial.println("left");
        servo.SetPosition(90 - degree);
    } else {
       // Serial.println("right");
        servo.SetPosition(90 + degree);
    }
}

void WindControl::setFanSpeed(uint16_t speed)
{
    int t_speed = speed;
    if (t_speed > maxFanKmh) {
        t_speed = maxFanKmh;
    }

    int maxFanSpeed = (255 * fanPercentage / 100);
    int mapped = map(t_speed, 0, maxFanKmh, 0, maxFanSpeed);
    //  Serial.println("*********");
    //  Serial.println("speed " + String(speed, 10));
    //  Serial.println("t_speed " + String(t_speed, 10));
    //  Serial.println("maxFanKmh " + String(maxFanKmh, 10));
    //  Serial.println("maxFanSpeed " + String(maxFanSpeed, 10));
    //  Serial.println("mapped " + String(mapped, 10));
    //  Serial.println("*********");
    fan.SetFanSpeed(mapped);
}

void WindControl::setSpeed(uint16_t speed)
{
    setFanSpeed(speed);

    openValve(speed);
}

void WindControl::setMaxFanKmh(uint16_t maxSpeed) {
    this->maxFanKmh = maxSpeed;
}

void WindControl::setFanPercentage(uint16_t fanPercentage) {
    this->fanPercentage = fanPercentage;
}

void WindControl::setMaxOpenValve(uint16_t maxSpeed) {
    this->maxValveSpeed = maxSpeed;
}

void WindControl::setSettings(uint16_t maxFanKmh, uint16_t fanPercentage, uint16_t valveMaxOpenSpeed) {
    this->setMaxFanKmh(maxFanKmh);
	this->setFanPercentage(fanPercentage);
	this->setMaxOpenValve(valveMaxOpenSpeed);
}