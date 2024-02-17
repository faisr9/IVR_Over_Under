#pragma once
#include "common_code/traditional_drive.h"
#include "comp_15/intake.h"
#include "comp_15/cata.h"
#include "comp_15/pneumatics.h"
#include "common_code/generic_rotation_digikey.h"
#include "comp_15/doinker.h"

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

extern pros::Motor doinker_motor;

// V5 Sensors //
extern pros::Imu imu;
extern pros::ADIEncoder vertical_track_adi;
extern pros::ADIEncoder horizontal_track_adi;
extern pros::ADIPotentiometer doinker_pot;

// Legacy Sensors //

// Legacy Sensors //
extern pros::ADIButton cata_limit;
extern pros::ADIPotentiometer doinker_pot;

// extern pros::ADIDigitalOut Floor_brake;
// extern pros::ADIDigitalOut Wings;
extern const char WINGS;
extern const char FLOOR_BRAKE;

// Traditional Drive
extern traditional_drive drive;
