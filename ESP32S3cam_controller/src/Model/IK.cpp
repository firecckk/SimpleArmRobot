#include "IK.h"

void calculateJointAngles(float x, float y, float phi, float * theta1, float * theta2, float * theta3) {
    
    float r_squared = x * x + y * y;
    float r = sqrt(r_squared);
    
    float a = atan2(y, x);
    //printf("a: %.2f\n", a);
    
    float temp1 = - l2 * l2 + l1 * l1 + r_squared;
    float temp2 = 2 * r * l1;
    float b = acos(temp1 / temp2);
    //printf("b: %.2f\n", b);
    
    *theta1 = b + a;
    
    float temp3 = l1 * l1 + l2 * l2 - r_squared;
    float temp4 = 2 * l1 * l2;
    *theta2 = acos(temp3 / temp4) - M_PI_2;
    
    *theta3 = phi/180.0*M_PI - *theta1 - *theta2 + M_PI_2;

    *theta1 = *theta1 * 180.0 / M_PI;
    *theta2 = *theta2 * 180.0 / M_PI;
    *theta3 = *theta3 * 180.0 / M_PI;
}