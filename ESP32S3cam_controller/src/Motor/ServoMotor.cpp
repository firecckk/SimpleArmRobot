#include "ServoMotor.h"
#include "PWMDriver.h"

ServoMotor::ServoMotor(uint8_t id, uint16_t t_pulse_min, uint16_t t_pulse_max): 
        id(id), t_pulse_min(t_pulse_min), t_pulse_max(t_pulse_max) {};

ServoMotor::~ServoMotor(){};

void ServoMotor::setAngle(float angle) {
    uint16_t pulselen = map(angle, 0, 180, t_pulse_min, t_pulse_max);
    PWMDriver::getInstance()->setPWM(id, pulselen);
};
