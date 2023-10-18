#include "common_code/example_subsystem.h"
#include "comp_15/subsystem_creation_test.h"


void my_function() {

    // it's only complaining when I try to actually use it which is inconvenient but at least it is complaining
    // SubsystemParent* test_parent = &(ExampleSubsystem());
    pros::Motor motor(99);
    std::string subsystem_name = "Example subsystem";
    SubsystemParent* test_parent = new ExampleSubsystem(subsystem_name, motor);

    std::cout << test_parent->get_subsystem_name() << std::endl;
    // would need an actual brain to test this, guess that's what I'm doing tomorrow :)

    delete test_parent;
}