#include "common_code/example_subsystem.h"
#include <iostream>

ExampleSubsystem::ExampleSubsystem(std::string subsystem_name, pros::Motor& motor): SubsystemParent(subsystem_name), subsystem_motor_(motor) {}

void ExampleSubsystem::stop() {
    subsystem_motor_.move_voltage(0);
}

// don't need to define get_subsystem_name because we can just use the version in SubsystemParent
