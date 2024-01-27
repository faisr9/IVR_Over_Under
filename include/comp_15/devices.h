#pragma once
#include "main.h"
#include "common_code/traditional_drive.h"
#include "intake.h"

// Forward declare all devices here so that all of the robot files and subsystem classes can access them //

// Controllers //
extern pros::Controller ctrl_master;

// Motors //
extern pros::Motor front_top_right;
extern pros::Motor front_bottom_right;
extern pros::Motor back_right;
extern pros::Motor front_top_left;
extern pros::Motor front_bottom_left;
extern pros::Motor back_left;

extern pros::MotorGroup right_drive;
extern pros::MotorGroup left_drive;

extern pros::Motor cata_right;
extern pros::Motor cata_left;

extern pros::MotorGroup cata;

extern pros::Motor intake_motor;

// V5 Sensors //
extern pros::Imu imu;

// Legacy Sensors //
extern pros::ADIButton cata_limit;
extern char floor_brake;
extern char wings;

// Traditional Drive
extern traditional_drive drive;