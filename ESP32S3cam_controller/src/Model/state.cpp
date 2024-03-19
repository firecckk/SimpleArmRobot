#include "state.h"

Vector3 state::orientation = {0, 0, 0};
Vector3 state::position = {0, 0, 0};
Vector3 state::movement = {0, 0, 0};

float state::get_pitch() {
    return state::orientation.x;
}

float state::get_yaw() {
    return state::orientation.y;
}

float state::get_roll() {
    return state::orientation.z;
}

float state::get_height() {
    return state::position.y;
}

float state::get_depth() {
    return state::position.z;
}

float state::get_move_x() {
    return state::movement.x;
}

float state::get_move_y() {
    return state::movement.y;
}