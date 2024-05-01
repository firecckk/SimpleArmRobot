#include "ArmState.h"

ArmState armState = FREE;

extern int filtered_adc;

extern bool is_motion_finished();

int adc_initial_value = 0; // this may solve EMI issue of board

int last_arm_state = 0;

void ArmState_update() {
        if(armState!=last_arm_state) {
          last_arm_state = armState;
          Serial.printf("arm state %d\n", armState);
        }
        switch(armState) {
        case FREE:
        state::require_update=true;
        state::update();
        if(state::grab==true) armState=GRABING;
        movement();
        break;
        case ALIGN_V:
        state::jointAngles[0]=90;
        state::jointAngles[1]=90;
        state::jointAngles[2]=0;
        state::jointAngles[3]=state::get_pitch();
        state::jointAngles[4]=state::get_roll();
        movement();
        break;
        case GRAB_V:
        state::jointAngles[0]=90;
        state::jointAngles[1]=65.7;
        state::jointAngles[2]=0;
        state::jointAngles[3]=state::get_pitch(); // 113
        state::jointAngles[4]=state::get_roll();
        break;
        case GRABING:
        armState = GRABING_LOOP;
        adc_initial_value = filtered_adc;
        case GRABING_LOOP:
        gripper_behavior(filtered_adc);
        break;
        case LIFTING1:
        state::jointAngles[0]=90;
        state::jointAngles[1]=90;
        state::jointAngles[2]=0;
        state::jointAngles[3]=90;
        state::jointAngles[4]=90;
        if(is_motion_finished()) armState=LIFTING2;
        break;
        case LIFTING2:
        state::jointAngles[0]=90;
        state::jointAngles[1]=90;
        state::jointAngles[2]=160;
        state::jointAngles[3]=180;
        state::jointAngles[4]=90;
        if(is_motion_finished()) armState=DROPING;
        break;
        case DROPING:
        state::jointAngles[5]=25;
        if(is_motion_finished()) armState=FREE;
        break;
    }
}

void gripper_behavior(int current_value)
{
  int increases = current_value - adc_initial_value;
  Serial.printf("filtered_adc=%d", current_value);
  if (!state::grab)
    adc_initial_value = current_value;
  else
  {
    if (increases > 80) {
      state::grab = false;
      armState = LIFTING1;
      }
    else
    {
      if (state::jointAngles[5] < 100) state::jointAngles[5] += 0.1;
    }
  }
}

void movement() {
      if(state::move_update) {
        state::move_update = false;
        Serial.printf("%d %d %d\n", (int) (state::get_move_x()*100), (int) (state::get_move_y()*100), (int) (state::get_move_roll()*100));
    }
}