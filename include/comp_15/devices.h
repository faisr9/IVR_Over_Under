#pragma once
#include "main.h"
#include "common_code/traditional_drive.h"

// Forward declare all devices here so that all of the robot files and subsystem classes can access them //

// Controllers //
extern pros::Controller ctrl_master;

// Motors //
extern pros::Motor FrontTopRight;
extern pros::Motor FrontBottomRight;
extern pros::Motor BackRight;
extern pros::Motor FrontTopLeft;
extern pros::Motor FrontBottomLeft;
extern pros::Motor BackLeft;

extern pros::MotorGroup RightDrive;
extern pros::MotorGroup LeftDrive;

extern pros::Motor cata_right;
extern pros::Motor cata_left;

extern pros::MotorGroup Cata;

extern pros::Motor Intake;

// V5 Sensors //
extern pros::Imu imu;
extern pros::ADIEncoder vertical_track;
extern pros::ADIEncoder horizontal_track;

// Legacy Sensors //
extern pros::ADIButton Cata_limit;
extern pros::ADIDigitalOut Floor_brake;
extern pros::ADIDigitalOut Wings;

// Other Classes //
extern traditional_drive tank_drive;