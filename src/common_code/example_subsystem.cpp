#include "common_code/example_subsystem.h"
#include <iostream>
#include "api.h"

// extern std::unique_ptr<ExampleSubsystem> ExampleSubsystem::instance_;
extern ExampleSubsystem* ExampleSubsystem::instance_ = nullptr;
// extern pros::MutexVar<ExampleSubsystem*> inst_two_(nullptr);


// note that this setup does not handle a lot of edge cases:
// What if we call getInstance before createInstance? Then nullptr is just returned
// What if createInstance is called twice? Then std::uniqueptr.reset() will free the memory for us
// In the deconstructor how do we even tell if instance_ has been created? unique_ptr figures that out for us

// silly c++ doesn't let you initialize static varaibles with values.


// want createInstance because want to be able to pass in objects subsystems need.
// If called a second time (or more) this function will do nothing (aka act just like getInstance() and only return)
ExampleSubsystem* ExampleSubsystem::createInstance(pros::Motor& motor) {
    pros::lcd::set_text(3, "In create");
    if (!instance_) {
        pros::lcd::set_text(4, "In conditional");
        instance_ = new ExampleSubsystem(motor);
        pros::lcd::set_text(4, "heap var made");

        // instance_.reset(temp);
        pros::lcd::set_text(4, "Made subsystem");
    }
    pros::lcd::set_text(3, "About to return");

    // return instance_.get();
    return instance_;
}

ExampleSubsystem* ExampleSubsystem::getInstance() {
    // TODO: handle case of instance_.get() == nullptr. Do we want to throw an error or someting?
    // but we won't want to crash the code during a potentially real run.
    // return instance_.get(); // will return nullptr if nothing stored

    // for now will put error to help with debugging
    // TO-DO: remove error once done debugging

    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

// TO-DO: add back parent class
ExampleSubsystem::ExampleSubsystem(pros::Motor& motor): SubsystemParent("ExampleSubsystem"), subsystem_motor_(motor) {
    pros::lcd::set_text(5, "In constructor");
}

ExampleSubsystem::~ExampleSubsystem() {
    pros::lcd::set_text(6, "DESTRUCTOR CALLED");

    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }

    // instance_.reset(); // will deallocate instance_ only if anything is stored
}

void ExampleSubsystem::stop() {
    subsystem_motor_.move_voltage(0);
}

void ExampleSubsystem::set_power(int power) {
    subsystem_motor_.move(power);

    if (instance_ == nullptr) {
        pros::lcd::set_text(6, "NOW NULLPTR");
    }
}


// don't need to define get_subsystem_name because we can just use the verison in SubsystemParent

