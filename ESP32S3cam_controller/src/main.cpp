#include "VideoWebServer/VideoWebServer.h"
#include "Motor/PWMDriver.h"
#include "Motor/ServoMotor.h"
#include "Controllers/MotionProfile.h"
#include "Controllers/BasicController.h"
#include "MqttClient/MqttClient.h"
#include "Model/state.h"
#include "ADC/ADC.h"
#include "ArmState.h"
#include "OLED/OLED.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SAMPLE_TIME 20 // ms

TaskHandle_t task2ms; // run adc every 2ms
void loop2ms(void *pvParameters);

// Motor calibration
ServoMotor joint0(0, 120, 500); // motor number and its calibration value(pulse count at 0 degree and pulse length at 180 degree)
ServoMotor joint1(1, 125, 475, true);
ServoMotor joint2(2, 135, 475);
ServoMotor joint3(3, 120, 480, true); // invert
ServoMotor joint4(4, 125, 510);
ServoMotor gripper(5, 120, 480);

// trapzoidal profile config;
MotionProfile p0(45, 60, 1, SAMPLE_TIME); // velocity, accelration, method=trapzoidal, sample time.
MotionProfile p1(45, 60, 1, SAMPLE_TIME);
MotionProfile p2(45, 60, 1, SAMPLE_TIME);
MotionProfile p3(50, 60, 1, SAMPLE_TIME);
MotionProfile p4(50, 100, 1, SAMPLE_TIME);
MotionProfile p5(20, 10, 0, SAMPLE_TIME);

BasicController *controller[6];

// set initial state for motion profile
void armInit() {
  // init controllers
  controller[0] = new BasicController(&joint0, &p0);
  controller[0]->setAngleRange(0, 180);
  controller[0]->setFixedAngle(90);
  controller[0]->moveTo(90);
  controller[1] = new BasicController(&joint1, &p1);
  controller[1]->setAngleRange(0, 180);
  controller[1]->setFixedAngle(180);
  controller[1]->moveTo(180);
  controller[2] = new BasicController(&joint2, &p2);
  controller[2]->setAngleRange(0, 180);
  controller[2]->setFixedAngle(0);
  controller[2]->moveTo(0);
  controller[3] = new BasicController(&joint3, &p3);
  controller[3]->setAngleRange(0, 180);
  controller[3]->setFixedAngle(0);
  controller[3]->moveTo(0);
  controller[4] = new BasicController(&joint4, &p4);
  controller[4]->setAngleRange(0, 180);
  controller[4]->setFixedAngle(90);
  controller[4]->moveTo(90);
  controller[5] = new BasicController(&gripper, &p5);
  controller[5]->setAngleRange(30, 100);
  controller[5]->setFixedAngle(25);
  controller[5]->moveTo(25);
}

void update_controller()
{
  for (uint8_t i = 0; i < 6; i++) controller[i]->moveTo(state::jointAngles[i]);
  for (uint8_t i = 0; i < 6; i++) controller[i]->update();
}

// default ready state
void armReset()
{
  state::jointAngles[0] = 90; // 90
  state::jointAngles[1] = 180; //10
  state::jointAngles[2] = 0; //15
  state::jointAngles[3] = 0; //175
  state::jointAngles[4] = 90;
  state::jointAngles[5] = 25;
  state::orientation.x = 90;
  state::orientation.y = 90;
  state::orientation.z = 90;
  state::position.y = 145;
  state::position.z = -100;
  state::require_update = true;
}

void setup()
{
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println("setup");
  // setup pwm module i2c pin
  PWMDriver::setup(21, 47); // SDA=21, SCL=47
  //OLED_setup(3, 46);
  armInit();
  armReset();
  update_controller();
  // 2ms timer setup
  xTaskCreatePinnedToCore(loop2ms, "adc2ms", 2048, NULL, 1, &task2ms, 1);
  // setup video stream service
  VideoWebServer_setup();
  // setup mqtt client
  MqttClientSetup();
}

// this function is only for test/debug purpose.
void tune_value()
{
  int which = (int)state::debug.x;
  switch (which)
  {
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
  case 6:
    if(state::debug.y=0) set_xclk_20MHZ();
    if(state::debug.y=1) set_xclk_40MHZ();
    break;
  }
}

bool is_motion_finished() {
  bool is_done = p0.getFinished() && p1.getFinished() && p2.getFinished() &&
              p3.getFinished() && p4.getFinished() && p5.getFinished();
  return is_done;
}
int filtered_adc = 0;
void loop2ms(void *pvParameters)
{
  while (true)
  {
    filtered_adc=ADC_filter(analogRead(1));
    //Serial.printf("%d,", newValue);
    //Serial.printf("%d,", filtered_adc);
    //Serial.println(angle);
    vTaskDelay(pdMS_TO_TICKS(2));
  }
}

void loop()
{
  ArmState_update();
  update_controller(); // apply math model to controller
  MqttClientLoop();    // receive message
  tune_value();
  //Serial.println("running");
  vTaskDelay(pdMS_TO_TICKS(SAMPLE_TIME)); // 20ms defined by trapzoidal model.
}