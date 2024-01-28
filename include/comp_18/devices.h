#pragma once
#include "main.h"

// Forward declare all devices here so that all of the robot files and subsystem classes can access them //

// Motors //
extern pros::Motor right_front_top; // 14
extern pros::Motor right_front_bottom;
extern pros::Motor right_middle; // might actually be port 13
extern pros::Motor right_back; // might actually be port 12

extern pros::Motor left_front_top; // 14
extern pros::Motor left_front_bottom; // one of 18, 19, or 20
extern pros::Motor left_middle; // one of 18, 19, or 20
extern pros::Motor left_back; // one of 18, 19, or 20

extern pros::Motor_Group left_drive_motors;
extern pros::Motor_Group right_drive_motors;

extern pros::Motor intake; // in = negative, out = positive

// V5 Sensors //
extern pros::IMU imu;
extern pros::Distance distance_sensor;
extern pros::Rotation radial_rot_sensor;
extern pros::Rotation transverse_rot_sensor; // reversed so that going right is positive

// Legacy Sensors //
