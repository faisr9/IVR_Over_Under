#include "comp_15/auton.h"
#include "comp_15/controls.h"

/* First method to run when program starts */
void initialize() {
	// pros::lcd::initialize(); // Temp until custom GUI
	// comp15link->init(); // Will address later
	imu.reset(); // Very important!!!
	horizontal_track_adi.reset();
	vertical_track_adi.reset();
	// gui::gui_init();	// Will address later
	pros::delay(4000);
}

/*
	Runs when robot is disabled from competition controller after driver/auton 
	Non-motor/Non-motion related tasks should be done here
*/
void disabled() 
{

}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	// Call auton selector to run selected auton
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	controls();
}