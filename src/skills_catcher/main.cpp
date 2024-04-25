#include "skills_catcher/skills.h"
#include "skills_catcher/controls.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	auto gears=GEARSET_06;
	left_middle_1.set_gearing(gears);
	left_middle_2.set_gearing(gears);
	right_middle_1.set_gearing(gears);
	right_middle_2.set_gearing(gears);
	imu.reset();
	while(imu.is_calibrating())
		Task::delay(10);
	imu.tare();
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(0,"Catcher Skills");

	while(1)
	{
		// straight_left.move_velocity(100);
		// straight_right.move_velocity(100);
		lcd::print(2,"Left middle: %i",(int)left_middle_2.get_actual_velocity());
		lcd::print(3,"Right middle: %i",(int)right_middle_2.get_actual_velocity());
		lcd::print(4,"Front left: %i",(int)front_left.get_actual_velocity());
		lcd::print(5,"Front right: %i",(int)front_right.get_actual_velocity());
		lcd::print(6,"Back left: %i",(int)back_left.get_actual_velocity());
		lcd::print(7,"Back right: %i",(int)back_right.get_actual_velocity());

		astdriveCatcher.run();
		Task::delay(10);
	}
}
