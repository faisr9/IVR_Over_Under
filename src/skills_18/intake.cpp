#include "skills_18/intake.h"

//Subsystem parent stuff

Intake* Intake::instance_ = nullptr;
const int Intake::default_rpm_ = 600; // max rpm of direct motor

Intake::~Intake(){
    //Subsystem parent
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }

    //Intake
    if (intake_motor_ != nullptr) {
        delete intake_motor_;
        intake_motor_ = nullptr;
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
    intake_motor_->brake();
}

//Intake
Intake::Intake(pros::Motor &motor) : SubsystemParent("Intake") {
    intake_motor_ = new pros::Motor(motor); //I assume this is why you stored it as a pointer?
    intake_motor_->set_brake_mode(BRAKETYPE_COAST);
}

// Sets power in rpm which will call set_power using a conversion to mV
void Intake::set_rpm(int rpm){
    // 12000 mV is the max voltage of the motor
    // 600 rpm is the max rpm of the motor
    set_power(rpm * 12000 / 600);
}

// Sets power in mV. Only this method moves the motor
void Intake::set_power(int power){
    intake_motor_->move_voltage(power);
}

// Uses the default power, the power set by set_power, or the power passed in
void Intake::toggle_on(int rpm = default_rpm_){
    set_rpm(rpm);
}