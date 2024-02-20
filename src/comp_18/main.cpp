#include "comp_18/controls.h"
#include "comp_18/auton.h"

extern LinkHelper* comp18link;

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	comp18link->init();
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
    pros::delay(3000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	if(gui::selected_auton == gui::AUTON_COMP) {
		auton18(45000, false);
	}
	else if(gui::selected_auton == gui::AUTON_SKILLS) {
		auton18(60000, true);
	}
	else {
		return;
	}

	pros::delay(5000); // just putting this here to make sure nothing weird happens that we don't want upon auton code ending
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	// Comp
    controls();

	// Skills
	// auton18(60000, true);
}
