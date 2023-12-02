#include "main.h"

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
	Start against left loading bar
	Rotate {x} degree
	Toggle Kickstand

	Wait for Skills 18 Robot to reach ready position

	for(int i = 0; i < 22, i++) {
		Lower Catapult
		Wait {?? seconds}
		Activate Catapult
	}

	Untoggle kickstand

	Move to right loading zone

	Rotate 180 degrees

	Back into right loading bar

	Rotate {x} degrees

	Toggle Kickstand

	for(int i = 0; i < 22, i++) {
		Launch Triball
		Lower Catapultf
		Wait {?? seconds}
	}

	Untoggle Kickstand

	*/
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(2,"15 Skills");

	while(1)
		Task::delay(1000);
}
