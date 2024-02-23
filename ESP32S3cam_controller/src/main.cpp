//#include <Arduino.h>
#include "VideoWebServer/VideoWebServer.h"
#include "Motor/PWMDriver.h"
#include "Motor/ServoMotor.h"
#include "Model/MotionProfile.h"
#include "BasicController.h"

#define SAMPLE_TIME 20 // ms

// Motor calibration
ServoMotor joint0(0, 100, 500); // motor number and its calibration value(pulse count at 0 degree and pulse length at 180 degree)
ServoMotor joint1(1, 100, 500);
ServoMotor joint2(2, 100, 500);
ServoMotor joint3(3, 100, 500);
ServoMotor joint4(4, 100, 500);
ServoMotor gripper(5, 100, 500);

// trapzoidal profile config;
MotionProfile p0(30, 10, 1, SAMPLE_TIME); // velocity, accelration, method=trapzoidal, sample time.
MotionProfile p1(30, 10, 1, SAMPLE_TIME);
MotionProfile p2(30, 10, 1, SAMPLE_TIME);
MotionProfile p3(30, 10, 1, SAMPLE_TIME);
MotionProfile p4(30, 10, 1, SAMPLE_TIME);
MotionProfile p5(30, 10, 1, SAMPLE_TIME);

BasicController * controller[6];

// default ready state
void armReset() {
  controller[0]->moveTo(90);
  controller[1]->moveTo(10);
  controller[2]->moveTo(15);
  controller[3]->moveTo(175);
  controller[4]->moveTo(90);
  controller[5]->moveTo(75);
}

void setup() {
  // setup video stream service
  VideoWebServer_setup();
  // setup pwm module i2c pin
  PWMDriver::setup(21, 47); // SDA=21, SCL=47
  // init controller
  controller[0] = new BasicController(&joint0, &p0);
  controller[1] = new BasicController(&joint1, &p1);
  controller[2] = new BasicController(&joint2, &p2);
  controller[3] = new BasicController(&joint3, &p3);
  controller[4] = new BasicController(&joint4, &p4);
  controller[5] = new BasicController(&gripper, &p5);
  armReset();
}

void loop() {
  for(uint8_t i=0; i<6; i++) controller[i]->update();
  delay(SAMPLE_TIME); // 20ms defined by trapzoidal model.
}