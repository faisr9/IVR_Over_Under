#include "comp_15/comp15_includeList.h"

/*
Entire thing exactly the same as example subsystem
*/
extern Intake* Intake::instance_ = nullptr;

Intake::~Intake(){
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

Intake* Intake::createInstance(pros::Motor& motor){
    if (!instance_) {
        instance_ = new Intake(motor);
    }

    return instance_;
}
Intake* Intake::getInstance(){
    if (instance_ == nullptr) {
        throw std::runtime_error("Intake: Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}
void Intake::stop(){
    intake_motor_.move_voltage(0);
}

//input is from -127 to 127
void Intake::set_power(int power){
    intake_motor_.move(power);
}