#include "comp_18/intake.h"

/*
Entire thing exactly the same as example subsystem
*/
Intake* Intake::instance_ = nullptr;

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
    intake_motor_.brake();
}

// The new motor velocity from -+-100, +-200, or +-600 depending on the motor's gearset
void Intake::set_velocity(int velocity){
    intake_motor_.move_velocity(velocity);
}