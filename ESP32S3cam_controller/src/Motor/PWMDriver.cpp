#include "PWMDriver.h"

PWMDriver* PWMDriver::singleton = nullptr;

PWMDriver::PWMDriver(Adafruit_PWMServoDriver pwm): pwm(pwm) {};

PWMDriver *PWMDriver::getInstance()
{
    return singleton;
};

void PWMDriver::setup(uint8_t Pin_SDA, uint8_t Pin_SCL)
{
    Wire.setPins(Pin_SDA, Pin_SCL);
    Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire); // default device address 0x40
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
    PWMDriver::singleton = new PWMDriver(pwm);
};

void PWMDriver::setPWM(uint8_t id, uint16_t pulse_len){
    pwm.setPWM(id, 0, pulse_len);
}