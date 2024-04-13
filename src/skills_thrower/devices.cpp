#include "skills_15/devices.h"

using namespace pros;

/* Asterisk Drive */
// Diagonal Motors
Motor front_left;
Motor front_right;
Motor back_left;
Motor back_right;

/* Catapult */
Motor cata_1;
Motor cata_2;
Rotation cata_rot_sensor;

/* Pnuematics */
const char LEFT_WING;
const char RIGHT_WING;

/* IMU */
IMU imu(21);

/* Distance Sensors */
Distance distance_sensor_1;
Distance distance_sensor_2;
Rotation radial_rot_sensor;
Rotation transverse_rot_sensor;