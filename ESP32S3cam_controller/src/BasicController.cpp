#include "BasicController.h"

BasicController::BasicController(ServoMotor * joint, MotionProfile * profile): joint(joint), profile(profile) {}

void BasicController::update() {
    current_angle = (float) profile->update(target_angle);
    joint->setAngle(current_angle);
}

void BasicController::moveTo(float angle) {
    target_angle = angle;
}

float BasicController::getCurrentAngle() {
    return(current_angle);
}