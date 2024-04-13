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

/* Catapult */
extern Motor cata_1;
extern Motor cata_2;
extern Rotation cata_rot_sensor;

/* Pnuematics */
extern const char LEFT_WING;
extern const char RIGHT_WING;

/* IMU */
extern IMU imu;

/* Distance Sensors */
extern Distance distance_sensor_1;
extern Distance distance_sensor_2;
extern Rotation radial_rot_sensor;
extern Rotation transverse_rot_sensor;