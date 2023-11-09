#include "comp_15/example_sub_test.h"
#include "common_code/example_subsystem.h"
#include "api.h"
#include <sstream>

void my_funct() {

    pros::lcd::set_text(2, "In function");

    pros::Motor test_mtr(8);

    ExampleSubsystem* instance = ExampleSubsystem::createInstance(test_mtr);

	const void* address = static_cast<const void*>(instance);
	std::stringstream ss;
	ss << address;  
	std::string name = ss.str(); 
	pros::lcd::set_text(5, "Earlier address = " + name);

    pros::lcd::set_text(2, "Instance created");

    if (instance == nullptr) {
        pros::lcd::set_text(6, "Instance IS NULLPTR");

    }

    // pros::lcd::set_text(6, instance->get_subsystem_name());
    instance->set_power(50);

    pros::delay(3000);

    instance->stop();

    pros::delay(1000);

    // // new stuff
    ExampleSubsystem* inst = ExampleSubsystem::getInstance();

    if (inst == nullptr) {
		pros::lcd::set_text(6, "NULLPTR");
	}
	inst->set_power(-50);

	pros::lcd::set_text(2, "Set the power to -50");


	pros::delay(1000);
	if (inst == nullptr) {
		pros::lcd::set_text(6, "NULLPTR");
	}

	try {
		inst->set_power(0);
	} catch (...) {
		pros::lcd::set_text(3, "CAUGHT AN ERROR");
	}
	pros::lcd::set_text(2, "Stopped it");

    pros::lcd::set_text(3, inst->get_subsystem_name());
}