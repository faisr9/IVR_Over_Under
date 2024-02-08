#include "comp_18/comp18_includeList.h"

#include "common_code/generic_rotation_vex_rot.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset(); // Very important!!!
	transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
	pros::delay(3000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	//1 tile is .61 meters (2 ft)
	std::vector<std::vector<double>> move1 = {{0.0, 0.0}, {0.0, 0.61*1.5}}; //Movement 1

	///////////////////////////////////////////////////////////////////////////
	//TODO1: Add includes for pure pursuit into comp18 bot (use comp15 as reference)
	//TODO2: Add path to push out the ball and score
	///////////////////////////////////////////////////////////////////////////////////

	std::vector<std::vector<double>> move2 = {move1.back(), {0.0, 0.61*2}, {1, 0.61*2}}; // Movement 2
	// std::vector<std::vector<double>> move3 = {move2.back(), {1, 0.61*2}, {1, 0.61*2}}; // Movement 3

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

	pros::lcd::set_text(1, "move 1 start");
	followPath(move1, tank_drive, 0, false);
	pros::lcd::set_text(1, "move 1 end");

	pros::delay(100);

	followPath(move2, tank_drive, 270, false);

	odom_task.suspend();
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {

}
