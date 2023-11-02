#include "comp_15/example_sub_test.h"
#include "common_code/example_subsystem.h"
#include "api.h"
#include <sstream>

void my_funct(int power) {

    pros::lcd::set_text(2, "In function");

    ExampleSubsystem* instance = ExampleSubsystem::getInstance();

	if (instance == nullptr) {
        pros::lcd::set_text(3, "Instance IS NULLPTR");
    } else {
		const void* address = static_cast<const void*>(instance);
		std::stringstream ss;
		ss << address;  
		std::string name = ss.str(); 
		pros::lcd::set_text(3, "Function address = " + name);
		instance->set_power(power);
		pros::delay(1000);
		instance->stop();
		pros::delay(2000);
	}

	pros::lcd::set_text(4, "Exiting Function");
}