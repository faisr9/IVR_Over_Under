#include "skills_thrower/skills.h"
#include "skills_thrower/controls.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
	pros::delay(3000);
	//x_drive_odom.initTracker(0, 0, 0);
	//pros::delay(50);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() 
{
	lcd::print(2,"Thrower Skills");
	//skills();
	// lcd::print(3,"LeftCata Velocity: %i", (int)cata_left.get_actual_velocity());
	// lcd::print(4,"RightCata Velocity: %i", (int)cata_right.get_actual_velocity());
	
	controls();
}

// 4 rot, 9.35
// 3, 7.13
