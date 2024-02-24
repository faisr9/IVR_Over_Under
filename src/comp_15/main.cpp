// #include "comp_15/controls.h"
// #include "main.h"
// #include "common_code/movement_tank.h"
// #include "auton.h"
// #include "comp_15/include_list.h"
#include "main.h" // Not needed, but present to show its included
#include "comp_15/auton.h"
#include "comp_15/controls.h"


extern LinkHelper* comp15link;

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	comp15link->init();
	imu.reset(); // Very important!!!
	horizontal_track_adi.reset();
	vertical_track_adi.reset();
	pros::delay(4000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() 
{
	if(CompetitionCatapult::getInstance()->get_switch_state())
		CompetitionCatapult::getInstance()->set_cata_mode("R");
}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	// COMP
	// auton_15(45000, false); // COMP

	// SKILLS
	skills_15(false);

	while (1) {
		// no work bc whatever internal task is running will still be happening
		// if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
		// 	skils_task.suspend(); // this will cause many problems and will only be done for testing
		// 	stopMotors(drive);
		// 	break;
		// }

		pros::delay(50);
	}

	// auton_15(20000, true); // TESTING
	pros::delay(5000); // just putting this here to make sure nothing weird happens that we don't want upon auton code ending
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {

	skills_15(true);

	controls(); // COMP


	// auton_15(60000, true); // SKILLS
}