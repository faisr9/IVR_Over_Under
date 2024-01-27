#include "comp_18/comp18_includeList.h"

/*
Entire thing exactly the same as example subsystem
*/
extern IntakeClass* IntakeClass::instance_ = nullptr;

IntakeClass::~IntakeClass(){
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

IntakeClass* IntakeClass::createInstance(pros::Motor& motor){
    if (!instance_) {
        instance_ = new IntakeClass(motor);
    }

    return instance_;
}
IntakeClass* IntakeClass::getInstance(){
    if (instance_ == nullptr) {
        throw std::runtime_error("Intake: Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}
void IntakeClass::stop(){
    intake_motor_.move_voltage(0);
}

//input is from -127 to 127
void IntakeClass::set_power(int power){
    intake_motor_.move(power);
}