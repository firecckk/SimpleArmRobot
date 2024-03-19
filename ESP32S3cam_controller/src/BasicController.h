#include "Motor/ServoMotor.h"
#include "Model/MotionProfile.h"

// a motor can switch between different type of controllers.
class BasicController {
    public:
        BasicController(ServoMotor * joint, MotionProfile * profile);
        ~BasicController();
        void update();
        void moveTo(float angle);
        float getCurrentAngle();

    private:
        ServoMotor * joint;
        MotionProfile * profile;
        float target_angle;
        float current_angle;
};