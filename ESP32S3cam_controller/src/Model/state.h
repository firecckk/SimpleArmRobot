#include "Vector3.h"
/*
 * This file defines the state of robot including arm and base.
 * The arm performs 5 dof.
 * The base has 2 dof.
 */
class state
{
public:
    static Vector3 orientation; // arm pitch, yaw, roll
    static Vector3 position;    // arm x, y, z
    static Vector3 movement;    // wheel move x, y

    static float get_pitch();
    static float get_yaw();
    static float get_roll();
    static float get_height();
    static float get_depth();
    static float get_move_x();
    static float get_move_y();
};