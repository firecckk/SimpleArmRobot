#include "BasicController.h"

BasicController::BasicController(ServoMotor * joint, MotionProfile * profile): joint(joint), profile(profile) {}

void BasicController::update() {
    current_angle = (uint8_t) profile->update(target_angle);
    joint->setAngle(current_angle);
}

void BasicController::moveTo(uint8_t angle) {
    target_angle = angle;
}

uint8_t BasicController::getCurrentAngle() {
    return(current_angle);
}