#include "comp_18/comp18_includeList.h"
#include "common_code/movement_tank.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
    pros::delay(3000);
    tank_drive_18.getOdom().initTracker(0, 0, 0);
    pros::delay(50);
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

	// Please note that tasks CANNNOT be run globally OR IN INITIALIZE() (this is based on experience from past years)
	// This can be started here in op_control for now, or later in auton (and then turned off at the end of auton and
	// started again here, if needed
	pros::Task odom_task{[=] {
		while (1) {
			tank_drive_18.getOdom().updatePosition();
			pros::delay(50);
		}
	}}; // lambda function with a task

    pros::lcd::set_text(1, "Starting path");

	followPath(path, tank_drive_18, 90, false, false, false, 0.5, 2.0, 75, 150);

	pros::lcd::set_text(1, "Done with path");

	pros::delay(100);

	// followPath(path_reversed, tank_drive_18, 0, true);

    while(true) {
        
        tank_drive_18.toggle_drive_mode();

        // pros::lcd::print(1,"X: %lf",odometer.getX());
        // pros::lcd::print(2,"Y: %lf",odometer.getY());
        // pros::lcd::print(3,"Heading: %lf",odometer.getHeading());
        // pros::lcd::print(6,"1 meter: %lf",odometer.toMeters(2302.94, 0.0248));
        delay(30);
    }
}
