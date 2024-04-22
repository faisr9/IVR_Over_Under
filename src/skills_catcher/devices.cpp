#include "skills_catcher/devices.h"

Controller ctrl_master(E_CONTROLLER_MASTER);

Motor front_left(13,1);
Motor front_right(1,1);
Motor back_left(20,0);
Motor back_right(6,0);
Motor left_middle_1(16,0);
Motor left_middle_2(17,0);
Motor right_middle_1(5,0);
Motor right_middle_2(7,0);
Motor_Group straight_left({left_middle_1, left_middle_2});
Motor_Group straight_right({right_middle_1, right_middle_2});
// Motor_Group* left_=&straight_left;
// Motor_Group* right_=&straight_right;

Imu imu(10);


asterisk_drive astdriveCatcher(ctrl_master, front_left, front_right, back_left, back_right,straight_right, straight_left, imu);

//asterisk_drive astdriveCatcher(ctrl_master, front_left, front_right, back_left, back_right,right_middle_1,right_middle_2,left_middle_1,left_middle_2, imu);