// Online C++ compiler to run C++ program online
#include <iostream>
#include <math.h>

// 定义机械臂结构体
struct ArmAngles {
    double theta1;
    double theta2;
    double theta3;
};

// 计算三个关节角度的函数
struct ArmAngles calculateArmAngles(double x, double y, double phi, double l1, double l2) {
    struct ArmAngles angles;
    
    double r_squared = x * x + y * y;
    double r = sqrt(r_squared);
    
    double a = atan2(y, x);
    printf("a: %.2f\n", a);
    
    double temp1 = - l2 * l2 + l1 * l1 + r_squared;
    double temp2 = 2 * r * l1;
    double b = acos(temp1 / temp2);
    printf("b: %.2f\n", b);
    
    angles.theta1 = b + a;
    //if(x>=0) angles.theta1 = b + a;
    //else angles.theta1 = M_PI_2 - a +b;
    
    double temp3 = l1 * l1 + l2 * l2 - r_squared;
    double temp4 = 2 * l1 * l2;
    angles.theta2 = acos(temp3 / temp4) - M_PI_2;
    
    angles.theta3 = phi - angles.theta1 - angles.theta2 + M_PI;
    
    return angles;
}

void calculateEndEffector(double theta1, double theta2, double l1, double l2, double *x, double *y) {
    *x = l1 * cos(theta1) + l2 * cos(theta1 + theta2 - M_PI_2);
    *y = l1 * sin(theta1) + l2 * sin(theta1 + theta2 - M_PI_2);
}

int main() {
     // 示例输入值
    double x = 0;
    double y = 1.5;
    double xy_squared = sqrt(x * x + y * y);
    
    double phi = 0.0;
    double l1 = 1.0;
    double l2 = 1.0;
    double r_min =sqrt(l1 * l1 + l2 * l2);
    double r_max = l1+l2;
    
    // 调用函数计算关节角度
    struct ArmAngles angles = calculateArmAngles(x, y, phi, l1, l2);
    
    double recovered_x, recovered_y;
    calculateEndEffector(angles.theta1, angles.theta2, l1, l2, &recovered_x, &recovered_y);
    
    // 打印结果
    printf("Input values:\n");
    printf("x: %.2f\n", x);
    printf("y: %.2f\n", y);
    printf("d: %.2f\n", xy_squared);
    printf("phi: %.2f\n", phi);
    printf("l1: %.2f\n", l1);
    printf("l2: %.2f\n", l2);
    printf("r min: %.2f\n", r_min);
    printf("r max: %.2f\n\n", r_max);
    
    printf("Calculated joint angles:\n");
    printf("theta1: %.2f degrees\n", angles.theta1 * 180.0 / M_PI);
    printf("theta2: %.2f degrees\n", angles.theta2 * 180.0 / M_PI);
    printf("theta3: %.2f degrees\n", angles.theta3 * 180.0 / M_PI);
    
    printf("recovered x: %.2f\n", recovered_x);
    printf("recovered y: %.2f\n", recovered_y);
    
    return 0;
}