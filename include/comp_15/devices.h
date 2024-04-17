#pragma once
#include "common_code/include_list.h"

// Forward declare all devices here so that all of the robot files and subsystem classes can access them //

extern pros::Controller ctrl_master;

// Motors //
extern pros::Motor right_front_top;
extern pros::Motor right_front_bottom;
extern pros::Motor right_middle;
extern pros::Motor right_back;

extern pros::Motor left_front_top;
extern pros::Motor left_front_bottom;
extern pros::Motor left_middle;
extern pros::Motor left_back;

extern pros::Motor_Group left_drive_motors;
extern pros::Motor_Group right_drive_motors;

extern pros::Motor intake;

// V5 Sensors //
extern pros::IMU imu;
extern pros::Distance distance_sensor; // inside the intake/catching funnel; detects if we have a triball
extern pros::ADIEncoder radial_rot_sensor; // parallel to the normal wheels
extern pros::ADIEncoder transverse_rot_sensor; // perpendicular to the normal wheels


extern traditional_drive tank_drive_15;
// Legacy Sensors //

extern pros::ADIButton cata_limit;
extern const char WINGS;
extern const char TOPHANG;
extern const char INTAKE;
extern const char SIDEHANG;

bool triBall();