#include "main.h"
#include "comp_15/example_sub_test.h"
#include "comp_15/example_test_two.h"
#include "common_code/example_subsystem.h"
#include <sstream>

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(0,"15 Comp");

	my_funct();

	// my_funct_two();

	lcd::print(2, "DONE");


	pros::Motor the_motor(8);

	ExampleSubsystem* inst = ExampleSubsystem::getInstance();


	// from https://stackoverflow.com/questions/7850125/convert-this-pointer-to-string
	// doesn't work :(
	const void* address = static_cast<const void*>(inst);
	std::stringstream ss;
	ss << address;  
	std::string name = ss.str(); 
	lcd::set_text(6, "Later address = " + name);

	// if (inst == nullptr) {
	// 	lcd::set_text(6, "NULLPTR");
	// }

	// inst->set_power(-50);

	// lcd::set_text(2, "Set the power to -50");


	// pros::delay(1000);
	// if (inst == nullptr) {
	// 	lcd::set_text(6, "NULLPTR");
	// }

	// try {
	// 	inst->set_power(0);
	// } catch (...) {
	// 	lcd::set_text(3, "CAUGHT AN ERROR");
	// 	pros::delay(1000);
	// }
	// lcd::set_text(2, "Stopped it");
	// pros::delay(1000);

	// lcd::set_text(3, inst->get_subsystem_name());

	// while(1)
	// 	Task::delay(1000);
}
