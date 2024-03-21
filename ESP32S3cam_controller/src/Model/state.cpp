#include "state.h"
#include "Arduino.h"

Vector3 state::orientation = {0, 0, 0};
Vector3 state::position = {0, 0, 0};
Vector3 state::movement = {0, 0, 0};
Vector3 state::debug = {0, 0, 0};
float state::jointAngles[] = {0, 0, 0, 0, 0, 0};
bool state::require_update = false;

float state::get_pitch() {
    return state::orientation.x;
}

float state::get_yaw() {
    return state::orientation.y;
}

float state::get_roll() {
    return state::orientation.z;
}

float state::get_height() {
    return state::position.y;
}

float state::get_depth() {
    return state::position.z;
}

float state::get_move_x() {
    return state::movement.x;
}

float state::get_move_y() {
    return state::movement.y;
}

float state::get_gripper_angle() {
    return state::jointAngles[5];
}

void state::update() {
    if(require_update) {
        float a1, a2, a3;
  Serial.print("inputs: ");Serial.printf("%f, %f, %f\n", state::get_depth(), state::get_height(), state::get_pitch());
        // apply inverse kinematic, converting state space to joint space
        calculateJointAngles(state::get_depth(), state::get_height(), state::get_pitch(), 
                            &a1, &a2, &a3);
        if(isnan(a1) || isnan(a2) || isnan(a3)) {
            require_update = false;
            return;}
        state::jointAngles[1] = a1;
        state::jointAngles[2] = a2;
        state::jointAngles[3] = a3;
        state::jointAngles[0] = state::get_yaw();
        state::jointAngles[4] = state::get_roll();

  Serial.print("0: ");Serial.println(state::jointAngles[0]);
  Serial.print("1: ");Serial.println(state::jointAngles[1]);
  Serial.print("2: ");Serial.println(state::jointAngles[2]);
  Serial.print("3: ");Serial.println(state::jointAngles[3]);
  Serial.print("4: ");Serial.println(state::jointAngles[4]);
  Serial.print("5: ");Serial.println(state::jointAngles[5]);
        require_update = false;
    }
}
