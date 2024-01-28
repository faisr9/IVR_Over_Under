#include "comp_15/comp15_includeList.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	Pneumatics::createInstance(WINGS, FLOOR_BRAKE);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
}

pros::Controller master(CONTROLLER_MASTER);
Pneumatics* pneumatics = Pneumatics::createInstance('A', 'G');

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {

	while(1){

		pros::lcd::set_text(2, std::to_string(pneumatics->getWings()->getStatus()));
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
			pneumatics->getWings()->toggle();
		}
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)){
			pneumatics->getWings()->off();
		}
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
			pneumatics->getWings()->on();
		}
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
			pneumatics->stop();
		}

		delay(50);
	}

}
