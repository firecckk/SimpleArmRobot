#include "ServoMotor.h"
#include "PWMDriver.h"

ServoMotor::ServoMotor(uint8_t id, uint16_t t_pulse_min, uint16_t t_pulse_max): 
        id(id), t_pulse_min(t_pulse_min), t_pulse_max(t_pulse_max) {};

ServoMotor::ServoMotor(uint8_t id, uint16_t t_pulse_min, uint16_t t_pulse_max, bool inverted): 
        id(id), t_pulse_min(t_pulse_min), t_pulse_max(t_pulse_max), inverted(inverted) {};

ServoMotor::~ServoMotor(){};

void ServoMotor::setInversion(bool invert) {
    inverted = invert;
};

void ServoMotor::setAngle(float angle) {
    uint16_t pulselen;
    if(inverted) pulselen = map(angle, 180, 0, t_pulse_min, t_pulse_max);
    else pulselen = map(angle, 0, 180, t_pulse_min, t_pulse_max);
    PWMDriver::getInstance()->setPWM(id, pulselen);
};
