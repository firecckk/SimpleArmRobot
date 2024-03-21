#include "Vector3.h"
#include "IK.h"
/*
 * This file defines the state of robot including arm and base.
 * The arm performs 5 dof.
 * The base has 2 dof.
 */
#ifndef STATE_H
#define STATE_H

class state
{
public:
    static Vector3 orientation; // arm pitch, yaw, roll
    static Vector3 position;    // arm x, y, z
    static Vector3 movement;    // wheel move x, y
    static Vector3 debug; // for debug purpose
    
    static float jointAngles[6];
    static bool require_update;

    static float get_pitch();
    static float get_yaw();
    static float get_roll();
    static float get_height();
    static float get_depth();
    static float get_move_x();
    static float get_move_y();
    static float get_gripper_angle();

    static void update(); // convert state space to joint space
};

#endif