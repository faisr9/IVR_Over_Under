#include "pookie/controls.h"
#include "pookie/auton.h"

// extern LinkHelper* comp15link;

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	// comp15link->init();
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
	Pneumatics::getInstance()->getWings()->off();
	Pneumatics::getInstance()->getIntake()->off();
	Pneumatics::getInstance()->getSideHang()->off();
	Pneumatics::getInstance()->getTopHang()->off();
	// gui::gui_init();
    pros::delay(3000);
	tank_drive_15.getOdom().initTracker(0, 0, 0);
	pros::delay(100);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	win_point_auton();
	ctrl_master.rumble("...__...");
	// if(gui::selected_auton == gui::AUTON_COMP) {
		// win_point_auton();
	// }
	// else do nothing. make sure to select the auton!
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	bool control_enable = true;

	pros::Task controlsTask {[=] {controls();}};

	while(1) {
		if(ctrl_master.get_digital_new_press(BUTTON_B)) {
			// control_enable = !control_enable;
			// if (control_enable) {
			// 	controlsTask.resume();
			// } else {
			// 	controlsTask.suspend();
			// }
			// ctrl_master.rumble(".");
			// delay(1000);
			
		}

		if(ctrl_master.get_digital_new_press(BUTTON_A)) {
			controlsTask.suspend();
			control_enable = false;
			for (int i = 0; i < 3; i++) {
				ctrl_master.rumble(".");
				delay(1000);
			}
			autonomous();
		}

		delay(100);
	}
}
