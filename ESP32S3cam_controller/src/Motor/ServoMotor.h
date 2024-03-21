#include <Arduino.h>

#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

class ServoMotor {
    private:
        uint8_t id; // which motor.
        bool inverted=false;

    public:
    ServoMotor(uint8_t id, uint16_t t_pulse_min, uint16_t t_pulse_max);
    ServoMotor(uint8_t id, uint16_t t_pulse_min, uint16_t t_pulse_max, bool inverted);
        uint16_t t_pulse_min, t_pulse_max; // the pulse length for 0 degree and 180 degree

    // Destructor
    ~ServoMotor();

    void setInversion(bool invert);

    void setAngle(float angle);

};

#endif