#include "Motor/ServoMotor.h"
#include "Model/MotionProfile.h"

// a motor can switch between different type of controllers.
class BasicController {
    public:
        BasicController(ServoMotor * joint, MotionProfile * profile);
        ~BasicController();
        void update();
        void moveTo(uint8_t angle);
        uint8_t getCurrentAngle();

    private:
        ServoMotor * joint;
        MotionProfile * profile;
        uint8_t target_angle;
        uint8_t current_angle;
};