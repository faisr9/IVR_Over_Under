#include "skills_catcher/skills.h"
#include "skills_catcher/controls.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset();
	while(imu.is_calibrating()) {
		pros::lcd::set_text(1, "IMU Calibrating!!!");
		Task::delay(25);
	}
	pros::lcd::set_text(1, "");
	imu.tare();
	// ast_odom.initTracker(0, 0, 0);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	// skills();

	// controls();
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	// skills();

	controls();
}
