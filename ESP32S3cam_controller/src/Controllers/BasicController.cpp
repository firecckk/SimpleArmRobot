#include "BasicController.h"

BasicController::BasicController(ServoMotor * joint, MotionProfile * profile): joint(joint), profile(profile) {
    setAngleRange(0, 180); // default angle range
}

void BasicController::setAngleRange(float min, float max) {
    min_angle  = min;
    max_angle = max;
}

void BasicController::update() {
    current_angle = (float) profile->update(target_angle);
    joint->setAngle(current_angle);
}

void BasicController::moveTo(float angle) {
    if(angle<min_angle) target_angle = min_angle;
    else if(angle>max_angle) target_angle = max_angle;
    else target_angle = angle;
}

float BasicController::getCurrentAngle() {
    return(current_angle);
}