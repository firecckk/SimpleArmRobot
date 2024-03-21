#include "../Motor/ServoMotor.h"
#include "MotionProfile.h"

// a motor can switch between different type of controllers.
class BasicController {
    public:
        BasicController(ServoMotor * joint, MotionProfile * profile);
        ~BasicController();
        void update();
        void moveTo(float angle);
        float getCurrentAngle();
        void setAngleRange(float min_angle, float max_angle);

    private:
        ServoMotor * joint;
        MotionProfile * profile;
        float target_angle;
        float current_angle;
        float min_angle;
        float max_angle;
};