#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


// Adafruit 16-channel PWM & Servo driver

class PWMDriver {
    private:
        // single instance
        static PWMDriver* singleton;
        Adafruit_PWMServoDriver pwm;
        // hiden constructor
        PWMDriver(Adafruit_PWMServoDriver pwm);

    public:
        // singleton instance getter
        static PWMDriver* getInstance();
        static void setup(uint8_t Pin_SDA, uint8_t Pin_SCL);
        void setPWM(uint8_t id, uint16_t pulse_len);
};