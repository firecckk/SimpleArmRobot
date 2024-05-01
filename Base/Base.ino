#include <AccelStepper.h>
#include <math.h>

#define MAX_SPEED 400
#define ACCELERATION 800
float AD;
float BC;
const byte BUFFER_SIZE = 64;
char buffer[BUFFER_SIZE];
byte index = 0;
int x = 0;
int y = 0;
int o = 0;

int value1 = 0;
int value2 = 0;
int value3 = 0;

// Voor de Arduino Uno + CNC shield V3
#define MOTOR_X_ENABLE_PIN 8
#define MOTOR_X_STEP_PIN 2
#define MOTOR_X_DIR_PIN 5

#define MOTOR_Y_ENABLE_PIN 8
#define MOTOR_Y_STEP_PIN 3
#define MOTOR_Y_DIR_PIN 6

#define MOTOR_Z_ENABLE_PIN 8
#define MOTOR_Z_STEP_PIN 4
#define MOTOR_Z_DIR_PIN 7

#define MOTOR_A_ENABLE_PIN 8
#define MOTOR_A_STEP_PIN 12
#define MOTOR_A_DIR_PIN 13

AccelStepper motorX(1, MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN);
AccelStepper motorY(1, MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN);
AccelStepper motorZ(1, MOTOR_Z_STEP_PIN, MOTOR_Z_DIR_PIN);
AccelStepper motorA(1, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN);

void setup()
{
   Serial.begin(9600);
   pinMode(MOTOR_X_ENABLE_PIN, OUTPUT);
   pinMode(MOTOR_Y_ENABLE_PIN, OUTPUT);
   pinMode(MOTOR_Z_ENABLE_PIN, OUTPUT);
   pinMode(MOTOR_A_ENABLE_PIN, OUTPUT);

   motorX.setEnablePin(MOTOR_X_ENABLE_PIN);
   motorX.setPinsInverted(false, false, true);
   motorX.setAcceleration(ACCELERATION);
   motorX.setMaxSpeed(MAX_SPEED);
   motorX.enableOutputs();

   motorY.setEnablePin(MOTOR_Y_ENABLE_PIN);
   motorY.setPinsInverted(false, false, true);
   motorY.setAcceleration(ACCELERATION);
   motorY.setMaxSpeed(MAX_SPEED);
   motorY.enableOutputs();

   motorZ.setEnablePin(MOTOR_Z_ENABLE_PIN);
   motorZ.setPinsInverted(false, false, true);
   motorZ.setAcceleration(ACCELERATION);
   motorZ.setMaxSpeed(MAX_SPEED);
   motorZ.enableOutputs();

   motorA.setEnablePin(MOTOR_A_ENABLE_PIN);
   motorA.setPinsInverted(false, false, true);
   motorA.setAcceleration(ACCELERATION);
   motorA.setMaxSpeed(MAX_SPEED);
   motorA.enableOutputs();
}

int max_speed;
void loop()
{
   //Serial.println("running");
   if (Serial.available() > 0)
   {
      // read the incoming byte:
      String inputString = Serial.readStringUntil('\n');

      //x = 0;
      //y = 0;
      //o = 0;

      if (sscanf(inputString.c_str(), "%d %d %d", &x, &y, &o) == 3)
      {

         //Serial.print("value x:");
         //Serial.print(x);
         //Serial.print("value y:");
         //Serial.println(y);
         //Serial.print("value o:");
         //Serial.println(o);
         //float angle = o / 100;

         //AD = sin(angle) * cos(M_PI_4) + cos(angle) * sin(M_PI_4);
         //BC = sin(angle) * sin(M_PI_4) - cos(angle) * cos(M_PI_4);

         //max_speed = max(abs(AD), abs(BC));
         //motorX.setSpeed((AD * MAX_SPEED) / (max_speed - x));
         //motorY.setSpeed((BC * MAX_SPEED) / (max_speed - x));
         //motorZ.setSpeed((BC * MAX_SPEED) / (max_speed - x));
         //motorA.setSpeed((AD * MAX_SPEED) / (max_speed - x));
         float LeftFrontWheel = y + x + o;
         float RightFrontWheel = y - x - o;
         float LeftBackWheel = y - x + o;
         float RightBackWheel = y + x - o;

         motorX.move(LeftFrontWheel/10);
         motorY.move(RightFrontWheel/10);
         motorZ.move(LeftBackWheel/10);
         motorA.move(RightBackWheel/10);

         //motorX.setSpeed(LeftFrontWheel);
         //motorY.setSpeed(RightFrontWheel);
         //motorZ.setSpeed(LeftBackWheel);
         //motorA.setSpeed(RightBackWheel);

         Serial.print("LeftFrontWheel");
         Serial.println(LeftFrontWheel);
         Serial.print("RightFrontWheel");
         Serial.println(RightFrontWheel);
         Serial.print("LeftBackWheel");
         Serial.println(LeftBackWheel);
         Serial.print("RightBackWheel");
         Serial.println(RightBackWheel);
      }
   }

   motorX.run();
   motorY.run();
   motorZ.run();
   motorA.run();
}
