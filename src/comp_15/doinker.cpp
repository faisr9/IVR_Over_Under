#include "comp_15/doinker.h"

DoinkerClass* DoinkerClass::instance_ = nullptr;

DoinkerClass* DoinkerClass::createInstance(pros::Motor& motor, pros::ADIPotentiometer& doinker_pot)
{
    if (instance_ == nullptr) {
        instance_ = new DoinkerClass(motor, doinker_pot);
    }
    return instance_;
}

DoinkerClass* DoinkerClass::getInstance()
{
    return instance_;
}

DoinkerClass::~DoinkerClass()
{
    if (instance_ != nullptr) {
        delete instance_;
    }
}

DoinkerClass::DoinkerClass (pros::Motor& subsystem_motor, pros::ADIPotentiometer& doinker_pot) : 
    SubsystemParent("DoinkerClass"), doinker_motor_(subsystem_motor), doinker_pot_(doinker_pot)
{
    doinker_motor_.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);   
}

void DoinkerClass::move_up()
{
    if (doinker_pot_.get_value() < doinker_pot_up && doinker_pot_.get_value() > doinker_pot_low) {
        doinker_motor_.move(-doinker_speed);
    } else if (doinker_pot_.get_value() > doinker_pot_up || doinker_pot_.get_value() < doinker_pot_low) {
        doinker_motor_.move(doinker_speed);
    } else {
        doinker_motor_.brake();
    }
}

void DoinkerClass::move_down()
{
    if (doinker_pot_.get_value() > doinker_pot_down || doinker_pot_.get_value() < doinker_pot_low) {
        doinker_motor_.move(doinker_speed);
    } else if (doinker_pot_.get_value() > doinker_pot_up || doinker_pot_.get_value() > 0) {
        doinker_motor_.move(-doinker_speed);
    } else {
        doinker_motor_.brake();
    }
}

void DoinkerClass::stop()
{

}

void DoinkerClass::doink()
{

}

void DoinkerClass::move_to_pos(int pos)
{

}

void DoinkerClass::stayUp()
{

}

void DoinkerClass::set_speed(int speed)
{

}

int DoinkerClass::get_pos()
{

}
