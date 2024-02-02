#include "comp_15/comp15_includeList.h"

#include "common_code/generic_rotation_digikey.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	pros::delay(1000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {

	std::vector<std::vector<double>> path = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}};

	horizontal_track.reset();
	vertical_track.reset();

	pros::delay(500);

	pros::lcd::set_text(4, "Hi");

	tank_drive.getOdom().initTracker(0, 0, 0);
	pros::delay(500);

	pros::Task odom_task{[=] {
		tank_drive.getOdom().updatePosition();
		pros::lcd::set_text(7, "In task");
		pros::delay(50);
	}}; // lambda function with a task

	while (1) {
		tank_drive.toggle_drive_mode(0);
		pros::delay(50);
	}

	// you can't pass in a function that's accessed through a pointer but you can do this apparently :)

	pros::lcd::set_text(1, "Starting path");
	//followPath(path, tank_drive, 270, false);
	//pros::lcd::set_text(2, "Done with path");

	//odom_task.suspend();
}
