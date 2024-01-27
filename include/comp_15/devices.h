#pragma once
#include "main.h"

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

// Legacy Sensors //
extern pros::ADIButton Cata_limit;
// extern pros::ADIDigitalOut Floor_brake;
// extern pros::ADIDigitalOut Wings;
extern const char WINGS;
extern const char FLOOR_BRAKE;