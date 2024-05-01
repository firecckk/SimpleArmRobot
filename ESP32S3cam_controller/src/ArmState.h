#pragma once
#include "Model/state.h"
#include "ADC/ADC.h"

enum ArmState {
    IDLE,
    FREE,
    ALIGN_V, // alignment state vertically
    GRAB_V,
    GRABING,
    GRABING_LOOP,
    LIFTING1,
    LIFTING2,
    DROPING
};

void ArmState_update();

void gripper_behavior(int current_value);

void movement();
