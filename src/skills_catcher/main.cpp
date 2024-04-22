#include "skills_catcher/skills.h"
#include "skills_catcher/controls.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset();
	Task::delay(3000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(2,"18 Skills");

	while(1)
	{
		// straight_left.move_velocity(100);
		// straight_right.move_velocity(100);
		lcd::print(3,"Left middle motor 1: %f",left_middle_1.get_actual_velocity());
		lcd::print(4,"Left middle motor 2: %f",left_middle_2.get_actual_velocity());
		lcd::print(4,"Right middle motor 1: %f",right_middle_1.get_actual_velocity());
		lcd::print(4,"Right middle motor 2: %f",right_middle_2.get_actual_velocity());
		lcd::print(5,"Front left: %f",front_left.get_actual_velocity());
		lcd::print(5,"Front right: %f",front_right.get_actual_velocity());
		lcd::print(5,"Back left: %f",back_left.get_actual_velocity());
		lcd::print(6,"Back right: %f",back_right.get_actual_velocity());

		astdriveCatcher.run();
		Task::delay(10);
	}
}
