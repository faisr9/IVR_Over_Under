#include "comp_15/controls.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	Pneumatics::createInstance(WINGS, FLOOR_BRAKE);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
}

pros::Controller master(CONTROLLER_MASTER);
Pneumatics* pneumatics = Pneumatics::createInstance('A', 'G');

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	controls();
}
