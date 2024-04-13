#include "skills_18/devices.h"

using namespace pros;

/* Asterisk Drive */
// Diagonal Motors
Motor front_left;
Motor front_right;
Motor back_left;
Motor back_right;

// Straight Motors
Motor left_middle_1;
Motor left_middle_2;
Motor right_middle_2;
Motor right_middle_2;

/* Pnuematics */
const char LEFT_WING;
const char RIGHT_WING; 
const char NET; 

/* IMU */
pros::IMU imu(21);

/* Distance Sensors */
pros::Distance distance_sensor_1;
pros::Distance distance_sensor_2;
pros::Rotation radial_rot_sensor;
pros::Rotation transverse_rot_sensor;