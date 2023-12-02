#include "main.h"
#include "skills_18/skills.h"
#include "skills_18/controls.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	/*
	Start at (5,2)

	Move to (3.5, 3)

	Rotate to face goal

	for(int i = 0; i < 22; i ++) {
		Back up
		Move Forward
		Push Triball
	} 

	Rotate to face goal

	for(int i = 0; i < 22; i ++) {
		Back up
		Move Forward
		Push Triball
	} 

	Move to blue vertical bar

	Climb
	*/
	skills();
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(3,"18 Skills");

	controls();
}
