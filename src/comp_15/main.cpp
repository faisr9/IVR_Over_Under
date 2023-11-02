#include "main.h"
#include "initializer.cpp"

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
	lcd::print(0,"15 Comp");

	ExampleSubsystem* inst = ExampleSubsystem::getInstance();

	pros::lcd::set_text(0, "Instance Created");

	// from https://stackoverflow.com/questions/7850125/convert-this-pointer-to-string
	const void* address = static_cast<const void*>(inst);
	std::stringstream ss;
	ss << address;  
	std::string name = ss.str(); 
	lcd::set_text(1, "Later address = " + name);

	my_funct(50);
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	ExampleSubsystem* inst = ExampleSubsystem::getInstance();
	my_funct(100);

	inst->set_power(50);
	pros::delay(2000);
	inst->stop();

}
