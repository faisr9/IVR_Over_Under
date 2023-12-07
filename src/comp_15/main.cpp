#include "comp_15/comp15_includeList.h"

/* First method to run when program starts */
// void initialize() {
// 	pros::lcd::initialize(); // Temp until custom GUI

// }

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {

	std::vector<std::vector<double>> path = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}};

	pros::lcd::set_text(1, "Starting path");
	followPath(path, tank_drive, 270, false);
	pros::lcd::set_text(2, "Done with path");
}
