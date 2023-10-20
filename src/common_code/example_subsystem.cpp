#include "common_code/example_subsystem.h"
#include <iostream>

// note that this setup does not handle a lot of edge cases:
// What if we call getInstance before createInstance? Then nullptr is just returned
// What if createInstance is called twice? Then std::uniqueptr.reset() will free the memory for us
// In the deconstructor how do we even tell if instance_ has been created? unique_ptr figures that out for us

// silly c++ doesn't let you initialize static varaibles with values.


// want createInstance because want to be able to pass in objects subsystems need.
ExampleSubsystem* ExampleSubsystem::createInstance(pros::Motor& motor) {
    instance_.reset(new ExampleSubsystem(motor));
    return instance_.get();
}

ExampleSubsystem* ExampleSubsystem::getInstance() {
    // TODO: handle case of instance_.get() == nullptr. Do we want to throw an error or someting?
    // but we won't want to crash the code during a potentially real run.
    return instance_.get(); // will return nullptr if nothing stored
}

ExampleSubsystem::ExampleSubsystem(pros::Motor& motor): SubsystemParent(kSubsystemName), subsystem_motor_(motor) {}

ExampleSubsystem::~ExampleSubsystem() {
    instance_.reset(); // will deallocate instance_ only if anything is stored
}

void ExampleSubsystem::stop() {
    subsystem_motor_.move_voltage(0);
}

// don't need to define get_subsystem_name because we can just use the verison in SubsystemParent

