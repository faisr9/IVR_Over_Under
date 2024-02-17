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

	// Comp
	// auton18(45000, false);

	// Skills
	auton18(60000, true);
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	// Comp
    // controls();

	// Skills
	auton18(60000, true);
}
