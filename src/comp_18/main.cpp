#include "comp_18/controls.h"
#include "comp_18/auton.h"
#include "comp_18/devices.h"

extern LinkHelper* comp18link;

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	lcd::print(1, "wow");
	comp18link->init();
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
	Pneumatics::getInstance()->setRight(0);
	Pneumatics::getInstance()->setLeft(0);
	//gui::gui_init();
    pros::delay(3000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	if(gui::selected_auton == gui::AUTON_COMP) {
		auton18();
	}
	// else do nothing. make sure to select the auton!
}
using namespace pros;
using namespace std;
/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(5, "X-Drive Test");

	while (1)
	{
		drive.run();
		delay(20);
	}
	// if(gui::selected_auton == gui::AUTON_COMP) {
	// 	controls();
	// }
	// else if(gui::selected_auton == gui::AUTON_SKILLS) {
	// 	skills18(true);
	// 	controls();
	// }
	// else {
	// 	controls();
	// }

	// Driver Skills
	// does not exist yet
}
