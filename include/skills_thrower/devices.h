#pragma once
#include "common_code/include_list.h"

// Forward declare all devices here so that all of the robot files and subsystem classes can access them //

// Controllers //
extern pros::Controller ctrl_master;

// Motors //
extern pros::Motor front_left;
extern pros::Motor front_right;
extern pros::Motor back_left;
extern pros::Motor back_right;

extern pros::Motor intake_motor;

// V5 Sensors //
extern pros::Imu imu;
extern pros::ADIEncoder vertical_track_adi;
extern pros::ADIEncoder horizontal_track_adi;

// Legacy Sensors //