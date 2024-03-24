// #include "main.h" // Not needed, but present to show its included
#include "comp_15/auton.h"
#include "comp_15/controls.h"

extern LinkHelper* comp15link;

/* First method to run when program starts */
void initialize() {
	// pros::lcd::initialize(); // Temp until custom GUI
	comp15link->init();
	testimu.reset(); // Very important!!!
	horizontal_track_adi.reset();
	vertical_track_adi.reset();
	gui::gui_init();
	pros::delay(4000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() 
{
	
}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	if(gui::selected_auton == gui::AUTON_COMP) {
		auton_15();
		// ctrl_master.rumble("---");
	}
	// else do nothing. make sure to select the auton!
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	controls();
}