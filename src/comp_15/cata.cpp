#include "comp_15/comp15_includeList.h"
#include <filesystem>

// note: velocity in rpm

CompetitionCatapult* CompetitionCatapult::createInstance(pros::MotorGroup& motorgroup, pros::ADIButton& killswitch, int load, int launch) {
    if (!instance_) {
        instance_ = new CompetitionCatapult(motorgroup, killswitch, load, launch);
    }
    return instance_;
}
CompetitionCatapult::CompetitionCatapult(pros::MotorGroup& motorgroup, pros::ADIButton& killswitch, int load, int launch) 
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
    motors.brake();
}

void CompetitionCatapult::prime() {
    while (!kill_switch.get_value()) {
        motors.move(load_voltage);
    }

    stop();
}

void CompetitionCatapult::cycle() {
    if (!kill_switch.get_value()) {
        prime();
    }
    release();
    prime();
}

void CompetitionCatapult::release() {
    while (kill_switch.get_value()) {
        motors.move(launch_voltage);
    }
    stop();
}