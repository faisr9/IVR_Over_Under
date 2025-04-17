#pragma once
#include "common_code/include_list.h"

using namespace pros;

/* Controllers */
extern Controller ctrl_master;

/* Asterisk Drive */
// Diagonal Motors
extern Motor front_left;
extern Motor front_right;
extern Motor back_left;
extern Motor back_right;

// Straight Motors
extern Motor left_middle_1;
extern Motor left_middle_2;
extern Motor right_middle_1;
extern Motor right_middle_2;

extern Motor_Group straight_left;
extern Motor_Group straight_right;

/* IMU */
extern IMU imu;

// /* Distance Sensors */
// extern Distance distance_sensor_1;
// extern Distance distance_sensor_2;
extern Rotation radial_rot_sensor;
extern Rotation transverse_rot_sensor;


/* asterisk drive */
extern asterisk_drive astdriveCatcher;
extern Odom ast_odom;

extern Piston catcher_wings;