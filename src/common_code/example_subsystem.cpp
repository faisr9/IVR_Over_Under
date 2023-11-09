#include "common_code/example_subsystem.h"
#include <iostream>
#include "api.h"

/*
- sets instance_ as nullptr for proper memory management
- 'extern' keyword is used to link instance_ back to the class variable
*/
extern ExampleSubsystem* ExampleSubsystem::instance_ = nullptr;

/*
- creates an object of the class in the heap
- sets instance_ to the address of the object created
- only activates if instance_ hasn't been initialized
*/
ExampleSubsystem* ExampleSubsystem::createInstance(pros::Motor& motor) {
    if (!instance_) {
        instance_ = new ExampleSubsystem(motor);
    }

    return instance_;
}


/* 
- returns instance_
- throws a runtime error if instance_
    - 'createInstance' hasn't been called
    - instance_ has been deallocated)
*/
ExampleSubsystem* ExampleSubsystem::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

//constructor initializes subsysteem_motor_ and also calls SubsystemParent constructor with subsystem name
//CHANGE STRING PASSED INTO SUBSYSTEM PARENT TO YOUR CLASS NAME
ExampleSubsystem::ExampleSubsystem(pros::Motor& motor): SubsystemParent("Example Subsystem"), subsystem_motor_(motor) {
    pros::lcd::set_text(5, "In constructor");
}

//destructor deallocates instance_
ExampleSubsystem::~ExampleSubsystem() {
    pros::lcd::set_text(6, "DESTRUCTOR CALLED");

    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

//stops subsystem_motor_
void ExampleSubsystem::stop() {
    subsystem_motor_.move_voltage(0);
}

//sets the power for subsystem_motor
void ExampleSubsystem::set_power(int power) {
    subsystem_motor_.move(power);

    if (instance_ == nullptr) {
        pros::lcd::set_text(6, "NOW NULLPTR");
    }
}

