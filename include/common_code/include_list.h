#pragma once

#define M_PI 3.14159265358979323846

#include "api.h" // DO NOT REMOVE //

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

#include <iostream>
#include <chrono>
#include <string>
#include <math.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

#include "common_code/subsystem_parent.h"

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

// Vex defined function, might as well have just in case
#define waitUntil(condition)                                                   \
  do {                                                                         \
    pros::delay(5);                                                            \
  } while (!(condition))

using namespace pros;
using namespace std;