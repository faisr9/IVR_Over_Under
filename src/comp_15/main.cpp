#include "comp_15/comp15_includeList.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	// CompetitionCatapult* competition_catapult_instance = CompetitionCatapult::getInstance();
	// competition_catapult_instance->load();

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
    CompetitionCatapult* competition_catapult_instance = CompetitionCatapult::createInstance(Cata, Cata_limit);
	// competition_catapult_instance->cycle();
	// pros::delay(5000);
	// competition_catapult_instance->cycle();
	// Cata.move(50);
	Controller master(E_CONTROLLER_MASTER);
    while (1){
        // intakeInstance->set_power(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
        if (master.get_digital(E_CONTROLLER_DIGITAL_A)){
            competition_catapult_instance->cycle();
        }
        if (master.get_digital(E_CONTROLLER_DIGITAL_X)){
            competition_catapult_instance->prime();
        }
        if (master.get_digital(E_CONTROLLER_DIGITAL_B)){
            competition_catapult_instance->release();
        }
        // else if (master.get_digital(E_CONTROLLER_DIGITAL_B)){
        //     intakeInstance->set_power(-12000);
        // }
        pros::delay(50);
    }
	
}
