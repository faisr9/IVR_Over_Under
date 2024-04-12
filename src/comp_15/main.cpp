// #include "comp_15/controls.h"
// #include "main.h"
// #include "common_code/movement_tank.h"
// #include "auton.h"
// #include "comp_15/include_list.h"
// #include "main.h" // Not needed, but present to show its included
#include "comp_15/auton.h"
#include "comp_15/controls.h"

extern LinkHelper* comp15link;

/* First method to run when program starts */
void initialize() {
	// pros::lcd::initialize(); // Temp until custom GUI
	comp15link->init();
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
	Pneumatics::getInstance()->getWings()->off();
	Pneumatics::getInstance()->getIntake()->off();
	Pneumatics::getInstance()->getSideHang()->off();
	Pneumatics::getInstance()->getTopHang()->off();
	gui::gui_init();
    pros::delay(3000);
}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	if(gui::selected_auton == gui::AUTON_COMP) {
		auton_15(45000, false);
		// ctrl_master.rumble("---");
	}
	else if(gui::selected_auton == gui::AUTON_SKILLS) {
		skills_15(false);
		// ctrl_master.rumble("-");
	}
	else {
		auton_15(45000, false);
		// ctrl_master.rumble("---");
	}
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	if(gui::selected_auton == gui::AUTON_COMP) {
		controls();
		// ctrl_master.rumble("---");
	}
	else if(gui::selected_auton == gui::AUTON_SKILLS) {
		skills_15(true);
		controls();
		// ctrl_master.rumble("-");
	}
	else {
		controls();
	}
}