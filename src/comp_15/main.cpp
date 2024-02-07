#include "comp_15/controls.h"
#include "main.h"

/* First method to run when program starts */
void initialize() {
    pros::lcd::initialize(); // Temp until custom GUI
    pros::lcd::print(7,"initialized");
	imu.reset();
    pros::delay(2000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	controls();
}
