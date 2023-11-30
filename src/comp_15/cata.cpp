#include "comp_15/comp15_includeList.h"

// note: velocity in rpm
extern CompetitionCatapult* CompetitionCatapult::instance_ = nullptr;

CompetitionCatapult* CompetitionCatapult::createInstance(pros::MotorGroup& motorgroup, pros::ADIDigitalIn& killswitch, int load, int launch) {
    if (!instance_) {
        instance_ = new CompetitionCatapult(motorgroup, killswitch, load, launch);
    }
    return instance_;
}
CompetitionCatapult::CompetitionCatapult(pros::MotorGroup& motorgroup, pros::ADIDigitalIn& killswitch, int load, int launch) 
    : SubsystemParent("Competition Catapult"), motors(motorgroup), kill_switch(killswitch),
        load_voltage(load), launch_voltage(launch) {
    
    pros::lcd::set_text(5, "In constructor");

}

CompetitionCatapult* CompetitionCatapult::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}
CompetitionCatapult::~CompetitionCatapult() {
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

void CompetitionCatapult::stop() {
    motors.move_voltage(0);
}

void CompetitionCatapult::set_power(int power) {
    launch_voltage = power;
}

void CompetitionCatapult::starting_position() {
    motors.move_absolute(0, 10);
}
void CompetitionCatapult::load() {
    while (!kill_switch.get_value()) {
        motors.move_voltage(-load_voltage);
    }
    stop();
}
void CompetitionCatapult::launch() {
    motors.move_voltage(launch_voltage);
}