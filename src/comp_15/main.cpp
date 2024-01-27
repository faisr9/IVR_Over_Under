#include "comp_15/comp15_includeList.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
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
	IntakeClass* intakeInstance = IntakeClass::createInstance(intakeMotor);
	while (1){
		// intakeInstance->set_power(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
		if (master.get_digital(E_CONTROLLER_DIGITAL_A)){
			intakeInstance->set_power(12000);
		}
		else if (master.get_digital(E_CONTROLLER_DIGITAL_B)){
			intakeInstance->set_power(-12000);
		}
		pros::delay(50);
	}
}
