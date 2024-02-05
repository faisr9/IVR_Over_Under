#include "comp_18/comp18_includeList.h"

#include "common_code/generic_rotation_vex_rot.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset(); // Very important!!!
	pros::delay(3000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	//1 tile is .61 meters (2 ft)
	std::vector<std::vector<double>> move1 = {{0.0, 0.0}, {0.0, 0.61*2.1}}; //Movement 1

	///////////////////////////////////////////////////////////////////////////
	//TODO1: Add includes for pure pursuit into comp18 bot (use comp15 as reference)
	//TODO2: Add path to push out the ball and score
	///////////////////////////////////////////////////////////////////////////////////

	std::vector<std::vector<double>> move2 = {move1.back(), {0.0, 0.8}, {0.0, 0.05}}; // Movement 2 (TODO2)


	pros::lcd::set_text(4, "Hi");

	tank_drive.getOdom().initTracker(0, 0, 0);
	pros::delay(50);

	pros::Task odom_task{[=] {
		while (1) {
			tank_drive.getOdom().updatePosition();
			pros::lcd::set_text(7, "In task");
			pros::delay(50);
		}
	}}; // lambda function with a task

	

	// you can't pass in a function that's accessed through a pointer but you can do this apparently :)

	pros::lcd::set_text(1, "Starting path");
	// turnToAngle(tank_drive, 179, 2, true, 2.5);
	followPath(move1, tank_drive, 0, false);
	pros::lcd::set_text(1, "Done with path");

	pros::delay(100);

	followPath(move2, tank_drive, -90, false);


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
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {

	Generic_Rotation* generic_vex_rot = new Generic_Rotation_VEX_Rot(transverse_rot_sensor, 1.96 * 0.0254 / 2);
	generic_vex_rot->initialize_sensor();
	double current_dist = 0;

	while (1) {

		if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)) {
			break;
		}

		double delta_distance = generic_vex_rot->get_meters_travelled();
		if (isnan(delta_distance)) {
			pros::lcd::set_text(3, "Delta distance is nan!!!!!!!");
		} else {
			pros::lcd::set_text(3, "");
			current_dist += delta_distance;
		}

		pros::lcd::set_text(2, "Current dist is " + std::to_string(current_dist));

		pros::delay(20);
	}

	delete generic_vex_rot;
}
