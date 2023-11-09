#include "comp_15/example_test_two.h"
#include "common_code/example_subsystem.h"


void my_funct_two() {
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
}