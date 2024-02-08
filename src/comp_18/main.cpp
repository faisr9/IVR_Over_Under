#include "comp_18/comp18_includeList.h"
#include "common_code/movement_tank.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
    pros::delay(3000);
    tank_drive_18.getOdom().initTracker(0, 0, 90);
    pros::delay(50);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	//1 tile is .61 meters (2 ft)
	double tile = 0.61;
	std::vector<std::vector<double>> move1 = {{0.0, 0.0}, {0.0, tile*1.5}}; //Movement 1

	///////////////////////////////////////////////////////////////////////////
	//TODO1: Add includes for pure pursuit into comp18 bot (use comp15 as reference)
	//TODO2: Add path to push out the ball and score
	///////////////////////////////////////////////////////////////////////////////////

	std::vector<std::vector<double>> move2 = {move1.back(), {0.0, tile*2}, {tile*0.5, tile*3},{1, tile*3}}; // Movement 2
	std::vector<std::vector<double>> move3 = {move2.back(), {tile*0.5, tile*3}}; // Movement 3
	std::vector<std::vector<double>> move4 = {move3.back(), {tile*0.5, tile*2}, {tile*1.5, tile*2}}; // Movement 4

	pros::delay(50);

	pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::lcd::set_text(7, "In task");
			pros::delay(50);
		}
	}}; // lambda function with a task

	
	// you can't pass in a function that's accessed through a pointer but you can do this apparently :)

	pros::lcd::set_text(1, "move 1 start");
	followPath(move1, drive, 90, false);
	pros::lcd::set_text(1, "move 1 end");

	pros::delay(100);

	pros::lcd::set_text(1, "move 2 start");
	followPath(move2, drive, 0, false);
	pros::lcd::set_text(1, "move 2 end");

	pros::delay(100);

	pros::lcd::set_text(1, "move 3 start");
	followPath(move3, drive, 0, true);
	pros::lcd::set_text(1, "move 3 end");

	pros::delay(100);

	pros::lcd::set_text(1, "move 4 start");
	followPath(move4, drive, 180, true);
	pros::lcd::set_text(1, "move 4 end");

	// pros::delay(100);

	// pros::lcd::set_text(1, "move 5 start");
	// followPath(move5, tank_drive, 270, true);
	// pros::lcd::set_text(1, "move 5 end");

	odom_task.suspend();
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	
    while(true) {
        tank_drive_18.toggle_drive_mode();
        delay(30);
    }
}
