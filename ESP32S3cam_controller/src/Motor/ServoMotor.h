#include <Arduino.h>

#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

class ServoMotor {
    private:
        uint8_t id; // which motor.
        uint16_t t_pulse_min, t_pulse_max; // the pulse length for 0 degree and 180 degree

    public:
    ServoMotor(uint8_t id, uint16_t t_pulse_min, uint16_t t_pulse_max);

    // Destructor
    ~ServoMotor();

    void setAngle(float angle);

};

#endif