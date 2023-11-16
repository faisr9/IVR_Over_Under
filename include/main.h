#pragma once
/**
 * \file main_master.h                 Hahaha
 *                                      VVVV
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
 * Add global defines/constants here;
 */
#define M_PI 3.14159265358979323846

/**
 * Add PROS header files here;
 * NOTE: There shouldn't be anything else here
 */
#include "api.h" // DO NOT REMOVE //
#include "pros/apix.h"
///////////////////////////////////////

/**
 * When multitasking, these values indicate the priority levels of tasks, and
 * how often they will run relative to other tasks.
 * 
 * Range; 2 (lowest) - 15 (highest)
 * IMPORTANT: Use these values to set task priorities
*/
#define TASK_PRIORITY_LOW 4
#define TASK_PRIORITY_MEDIUM_LOW 7
#define TASK_PRIORITY_MEDIUM_HIGH 11
#define TASK_PRIORITY_HIGH 14
///////////////////////////////////////

/**
 * Add other header files here;
 * Add the common code headers here
 */
#include "common_code/subsystem_parent.h"
#include "common_code/example_subsystem.h"
#include "common_code/x_drive.h"
///////////////////////////////////////

/**
 * Add C++ headers here
 */
#include <iostream>
#include <chrono>
#include <string>
#include <math.h>
#include <vector>
#include <fstream>


///////////////////////////////////////

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 */
#define ILLINI_SIMPLE_NAMES

#ifdef ILLINI_SIMPLE_NAMES
#define CONTROLLER_MASTER   pros::E_CONTROLLER_MASTER
#define CONTROLLER_PARTNER  pros::E_CONTROLLER_PARTNER
#define GEARSET_06          pros::E_MOTOR_GEARSET_06
#define GEARSET_18          pros::E_MOTOR_GEARSET_18
#define GEARSET_36          pros::E_MOTOR_GEARSET_36
#define BRAKETYPE_BRAKE     pros::E_MOTOR_BRAKE_BRAKE
#define BRAKETYPE_COAST     pros::E_MOTOR_BRAKE_COAST
#define BRAKETYPE_HOLD      pros::E_MOTOR_BRAKE_HOLD
#define RIGHT_X_AXIS        pros::E_CONTROLLER_ANALOG_RIGHT_X
#define RIGHT_Y_AXIS        pros::E_CONTROLLER_ANALOG_RIGHT_Y
#define LEFT_X_AXIS         pros::E_CONTROLLER_ANALOG_LEFT_X
#define LEFT_Y_AXIS         pros::E_CONTROLLER_ANALOG_LEFT_Y
#define BUTTON_A            pros::E_CONTROLLER_DIGITAL_A
#define BUTTON_B            pros::E_CONTROLLER_DIGITAL_B
#define BUTTON_X            pros::E_CONTROLLER_DIGITAL_X
#define BUTTON_Y            pros::E_CONTROLLER_DIGITAL_Y
#define BUTTON_UP           pros::E_CONTROLLER_DIGITAL_UP
#define BUTTON_DOWN         pros::E_CONTROLLER_DIGITAL_DOWN
#define BUTTON_LEFT         pros::E_CONTROLLER_DIGITAL_LEFT
#define BUTTON_RIGHT        pros::E_CONTROLLER_DIGITAL_RIGHT
#define BUTTON_R1           pros::E_CONTROLLER_DIGITAL_R1
#define BUTTON_R2           pros::E_CONTROLLER_DIGITAL_R2
#define BUTTON_L1           pros::E_CONTROLLER_DIGITAL_L1
#define BUTTON_L2           pros::E_CONTROLLER_DIGITAL_L2
#else
// Use PROS Default Simple Name List //
#define PROS_USE_SIMPLE_NAMES
#endif

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
// #define PROS_USE_LITERAL

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
using namespace pros;
// using namespace pros::literals;
// using namespace okapi;

/**
 * By redefining the competition methods allow them to be accessible from
 * the entire project.
 * An example is calling the autonomous from a button press to test it.
*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 * 
 * Runs everytime program is called
 */
void initialize(void);

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 * 
 * Runs only if connected to Field Network or Competition Switch
 */
void competition_initialize(void);

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 * 
 * Runs if field controller or competition switch sets mode to disable
 */
void disabled(void);

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 * 
 * Runs if Competition Switch or Field Network start autonomous
 */
void autonomous(void);

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 * 
 * Runs if Competition Switch or Field Network start driver control
 */
void opcontrol(void);

#ifdef __cplusplus
}
#endif

#endif /* PROS_MAIN_H */