#include "comp_18/controls.h"
#include "main.h"
#include "common_code/movement_tank.h"
#include "comp_18/auton.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
    pros::delay(3000);
    tank_drive_18.getOdom().initTracker(.61*2.15, .61*.5, 90);
    pros::delay(50);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {

	pros::delay(50);

	pros::Task odom_task{[=] {
		while (1) {
			tank_drive_18.getOdom().updatePosition();
			pros::lcd::set_text(7, "A: " + std::to_string(tank_drive_18.getOdom().getHeading()));
			pros::delay(50);
		}
	}}; // lambda function with a task
	
	auton18();

	odom_task.suspend();
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
    controls();
}
