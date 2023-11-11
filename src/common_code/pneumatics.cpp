#include "common_code/pneumatics.h"
#include <iostream>
#include "api.h"

/*
- sets instance_ as nullptr for proper memory management
- 'extern' keyword is used to link instance_ back to the class variable
*/
extern Pneumatics* Pneumatics::instance_ = nullptr;

/*
- creates an object of the class in the heap
- sets instance_ to the address of the object created
- only activates if instance_ hasn't been initialized
*/
Pneumatics* Pneumatics::createInstance(pros::ADIDigitalOut& piston) {
    if (!instance_) {
        instance_ = new Pneumatics(piston);
    }

    return instance_;
}


/* 
- returns instance_
- throws a runtime error if instance_
    - 'createInstance' hasn't been called
    - instance_ has been deallocated)
*/
Pneumatics* Pneumatics::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

//constructor initializes subsysteem_motor_ and also calls SubsystemParent constructor with subsystem name
Pneumatics::Pneumatics(pros::ADIDigitalOut& piston): SubsystemParent("Pneumatics"), piston1(piston) {
    pros::lcd::set_text(5, "In constructor");
}

//destructor deallocates instance_
Pneumatics::~Pneumatics() {
    pros::lcd::set_text(6, "DESTRUCTOR CALLED");

    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

void Pneumatics::on() {
    piston1.set_value(true);
}

void Pneumatics::off() {
    piston1.set_value(false);
}

void Pneumatics::stop() {
    piston1.set_value(false);
}
