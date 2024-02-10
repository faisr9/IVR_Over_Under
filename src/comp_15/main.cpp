#include "comp_15/controls.h"
#include "main.h"
#include "common_code/movement_tank.h"
#include "auton.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
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
	/*
	Example odometry task, leaving this here because this bit of code is quite important

	pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::lcd::set_text(7, "In task");
			pros::delay(50);
		}
	}}; // lambda function with a task

	// do something noteworthy

	odom_task.suspend();
	*/

	auton_15();

	// skills_15();
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	// auton_15();
	// skills_15();
	controls();
}