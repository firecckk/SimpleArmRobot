//#include <Arduino.h>
#include "VideoWebServer/VideoWebServer.h"
#include "Motor/PWMDriver.h"
#include "Motor/ServoMotor.h"
#include "Controllers/MotionProfile.h"
#include "Controllers/BasicController.h"
#include "MqttClient/MqttClient.h"
#include "Model/state.h"

#define SAMPLE_TIME 20 // ms

// Motor calibration
ServoMotor joint0(0, 120, 475); // motor number and its calibration value(pulse count at 0 degree and pulse length at 180 degree)
ServoMotor joint1(1, 125, 475, true);
ServoMotor joint2(2, 135, 475);
ServoMotor joint3(3, 110, 480, true); //invert
ServoMotor joint4(4, 125, 510);
ServoMotor gripper(5, 100, 500);

// trapzoidal profile config;
MotionProfile p0(80, 100, 1, SAMPLE_TIME); // velocity, accelration, method=trapzoidal, sample time.
MotionProfile p1(80, 100, 1, SAMPLE_TIME);
MotionProfile p2(80, 100, 1, SAMPLE_TIME);
MotionProfile p3(80, 100, 1, SAMPLE_TIME);
MotionProfile p4(80, 100, 1, SAMPLE_TIME);
MotionProfile p5(80, 100, 1, SAMPLE_TIME);

BasicController * controller[6];

// default ready state
void armReset() {
  //state::jointAngles[0] = 90;//90
  //state::jointAngles[1] = 180; //10
  //state::jointAngles[2] = 0; //15
  //state::jointAngles[3] = 0; //175
  //state::jointAngles[4] = 90;
  state::jointAngles[5] = 75;
  state::orientation.x = 90;
  state::orientation.y = 90;
  state::orientation.z = 90;
  state::position.y = 145;
  state::position.z = -100;
  state::require_update = true;
}

void setup() {
  // setup video stream service
  VideoWebServer_setup();
  // setup mqtt client
  MqttClientSetup();
  // setup pwm module i2c pin
  PWMDriver::setup(21, 47); // SDA=21, SCL=47
  // init controllers
  controller[0] = new BasicController(&joint0, &p0);
  controller[1] = new BasicController(&joint1, &p1);
  controller[2] = new BasicController(&joint2, &p2);
  controller[3] = new BasicController(&joint3, &p3);
  controller[4] = new BasicController(&joint4, &p4);
  controller[5] = new BasicController(&gripper, &p5);
  controller[5]->setAngleRange(20, 80);
  armReset();
}

void update_controller() {
  controller[0]->moveTo(state::jointAngles[0]);
  controller[1]->moveTo(state::jointAngles[1]);
  controller[2]->moveTo(state::jointAngles[2]);
  controller[3]->moveTo(state::jointAngles[3]);
  controller[4]->moveTo(state::jointAngles[4]);
  controller[5]->moveTo(state::jointAngles[5]);
  for(uint8_t i=0; i<6; i++) controller[i]->update();
}

// this function is only for test purpose.
void calibrate_motor() {
  int which = (int) state::debug.x;
  switch(which) {
    case 0:
    joint0.t_pulse_max = state::debug.z;
    joint0.t_pulse_min = state::debug.y;
    break;
    case 1:
    joint1.t_pulse_max = state::debug.z;
    joint1.t_pulse_min = state::debug.y;
    break;
    case 2:
    joint2.t_pulse_max = state::debug.z;
    joint2.t_pulse_min = state::debug.y;
    break;
    case 3:
    joint3.t_pulse_max = state::debug.z;
    joint3.t_pulse_min = state::debug.y;
    break;
    case 4:
    joint4.t_pulse_max = state::debug.z;
    joint4.t_pulse_min = state::debug.y;
    break;
    case 5:
    gripper.t_pulse_max = state::debug.z;
    gripper.t_pulse_min = state::debug.y;
    break;
  }
}

void loop() {
  MqttClientLoop();
  state::update();
  update_controller();
  calibrate_motor();
  delay(SAMPLE_TIME); // 20ms defined by trapzoidal model. 
}