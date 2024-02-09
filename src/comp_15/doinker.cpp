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
    
    doinkerState = false;
    doinkerRunning = false;
}

void DoinkerClass::move(doinker_move_t move)
{
    if (move == UP)
    {
        if (doinker_pot_.get_value() > doinker_pot_up && doinker_pot_.get_value() < doinker_pot_max) // Acts as a mutex
            return;
        else if (doinker_pot_.get_value() > 0 && doinker_pot_.get_value() < doinker_pot_low)
        {
            // Doinker too far up
            while(doinker_pot_.get_value() > doinker_pot_up || doinker_pot_.get_value() < doinker_pot_low)
            {
                doinker_motor_.move(kDOINKER_DOWN_SPEED);
                pros::delay(5);
            }
        }
        else
        {
            // Doinker is down
            while(doinker_pot_.get_value() < doinker_pot_up && doinker_pot_.get_value() > doinker_pot_low)
            {
                doinker_motor_.move(kDOINKER_UP_SPEED);
                pros::delay(5);
            }
        }
        doinker_motor_.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        doinker_motor_.brake();
        doinker_motor_.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    }
    else if (move == DOWN)
    {
        while(doinker_pot_.get_value() > doinker_pot_down || doinker_pot_.get_value() < doinker_pot_low)
        {
            doinker_motor_.move(kDOINKER_DOWN_SPEED);
            pros::delay(5);
        }

        doinker_motor_.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        doinker_motor_.brake();
        doinker_motor_.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    }
}

void DoinkerClass::stop()
{
    doinker_motor_.brake();
}

void DoinkerClass::doink()
{
    move(doinker_move_t::UP);
    delay(250);
    move(doinker_move_t::DOWN);
}

// void DoinkerClass::set_speed(int speed)
// {
//     doinker_speed = speed;
// }

int DoinkerClass::get_pos()
{
    return doinker_pot_.get_value();
}