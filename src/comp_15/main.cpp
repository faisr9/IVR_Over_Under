#include "comp_15/controls.h"
#include "main.h"
#include "common_code/movement_tank.h"
#include "auton.h"

extern LinkHelper* comp15link;

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	comp15link->init();
	imu.reset(); // Very important!!!
	horizontal_track_adi.reset();
	vertical_track_adi.reset();
	pros::delay(3000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	// COMP
	// auton_15(45000, false); // COMP

	// SKILLS
	auton_15(60000, true);

	// auton_15(20000, true); // TESTING
	pros::delay(5000); // just putting this here to make sure nothing weird happens that we don't want upon auton code ending
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	// controls(); // COMP

	auton_15(60000, true); // SKILLS
}