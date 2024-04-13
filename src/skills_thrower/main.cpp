#include "skills_thrower/skills.h"
#include "skills_thrower/controls.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(2,"Thrower Skills");

	while(1)
		Task::delay(1000);
}
