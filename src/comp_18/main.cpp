#include "comp_18/controls.h"
#include "comp_18/auton.h"

extern LinkHelper* comp18link;

/* First method to run when program starts */
void initialize() {
	// pros::lcd::initialize(); // Temp until custom GUI
	comp18link->init();
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
	Pneumatics::getInstance()->setRight(0);
	Pneumatics::getInstance()->setLeft(0);
	gui::gui_init();
    pros::delay(3000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	if(gui::selected_auton == gui::AUTON_COMP) {
		auton18(45000, false);
	}
	else if(gui::selected_auton == gui::AUTON_SKILLS) {
		skills18(false);
	}
	else {
		auton18(45000, false);
	}
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	if(gui::selected_auton == gui::AUTON_COMP) {
		controls();
	}
	else if(gui::selected_auton == gui::AUTON_SKILLS) {
		skills18(true);
		controls();
	}
	else {
		pros::Task odom_task{[=] {
			while (1) {
				tank_drive_18.getOdom().updatePosition();
				pros::delay(50);
			}
		}}; // lambda function with a task

		controls();
	}
}
