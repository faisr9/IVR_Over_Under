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
	pros::Task skils_task{[=] {
		skills_15();
	}};

	while (1) {
		if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
			skils_task.suspend(); // this will cause many problems and will only be done for testing
			stopMotors(drive);
			break;
		}

		pros::delay(50);
	}

	// auton_15(20000, true); // TESTING
	pros::delay(5000); // just putting this here to make sure nothing weird happens that we don't want upon auton code ending
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {

	drive.getOdom().initTracker(0, 0, 0);

	pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::delay(50);
		}
	}}; // lambda function with a task


	controls(); // COMP

	// auton_15(60000, true); // SKILLS
}