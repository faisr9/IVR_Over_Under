#include "comp_15/controls.h"
#include "main.h"
#include "common_code/movement_tank.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset(); // Very important!!!
	horizontal_track.reset();
	vertical_track.reset();
	pros::delay(3000);
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

	std::vector<std::vector<double>> path = {{0.0, 0.0}, {0.0, 0.8}, {1.0, 0.8}};
	std::vector<std::vector<double>> path_reversed = {path.back(), {0.0, 0.8}, {0.0, 0.05}};


	pros::lcd::set_text(4, "Hi");

	drive.getOdom().initTracker(0, 0, 0);
	pros::delay(50);

	pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::lcd::set_text(7, "In task");
			pros::delay(50);
		}
	}}; // lambda function with a task

	

	// you can't pass in a function that's accessed through a pointer but you can do this apparently :)

	pros::lcd::set_text(1, "Starting path");
	// turnToAngle(tank_drive, 179, 2, true, 2.5);

	followPath(path, drive, 90, false);

	pros::lcd::set_text(1, "Done with path");

	pros::delay(100);

	followPath(path_reversed, drive, 0, true);


	// int move_vel = 180;

	// while (1) {
	// 	// moveMotors(tank_drive, move_vel, -move_vel);
	// 	LeftDrive.move_velocity(move_vel);
	// 	RightDrive.move_velocity(-move_vel);
	// 	pros::lcd::set_text(5, "imu heading: " + std::to_string(imu.get_heading()));

	// 	if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
	// 		move_vel = 0;
	// 	}
	// 	pros::delay(50);
	// }

	// LeftDrive.move_velocity(0);
	// RightDrive.move_velocity(0);

	odom_task.suspend();

	while (1) {
		drive.toggle_drive_mode();
		// pros::lcd::set_text(5, "imu heading: " + std::to_string(imu.get_heading()));

		// pros::lcd::set_text(3, "hori " + std::to_string(horizontal_track.get_value()));
		// pros::lcd::set_text(4, "vert " + std::to_string(vertical_track.get_value()));
		pros::delay(50);
	}

	controls();
}
