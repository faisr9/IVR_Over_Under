#include "comp_15/comp15_includeList.h"
#include "pros/rtos.hpp"

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
	CompetitionCatapult* competition_catapult_instance = CompetitionCatapult::getInstance();
	competition_catapult_instance->cycle();
	pros::delay(5000);
	competition_catapult_instance->cycle();
	// Cata.move(50);
	
	
}
