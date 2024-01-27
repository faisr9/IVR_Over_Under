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

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	Controller master(E_CONTROLLER_MASTER);
	pros::Motor intakeMotor = pros::Motor(14);
	Pneumatics* pneumaticsInstance = Pneumatics::getInstance();
	while (1){
		if (master.get_digital(E_CONTROLLER_DIGITAL_A)){
			pneumaticsInstance->getFloorBrake()->toggle();
		}
		if (master.get_digital(E_CONTROLLER_DIGITAL_B)){
			pneumaticsInstance->getWings()->toggle();
		}
		pros::delay(50);
	}
}
