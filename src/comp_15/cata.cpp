#include "comp_15/comp15_includeList.h"


// note: velocity in rpm

CompetitionCatapult* CompetitionCatapult::createInstance(pros::MotorGroup& motorgroup, pros::ADIButton& killswitch) {
    if (!instance_) {
        instance_ = new CompetitionCatapult(motorgroup, killswitch);
    }
    return instance_;
}
CompetitionCatapult::CompetitionCatapult(pros::MotorGroup& motorgroup, pros::ADIButton& killswitch) 
    : SubsystemParent("Competition Catapult"), motors(motorgroup), kill_switch(killswitch){
    
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
        motors.move(cata_voltage);
    }

    stop();
}
//test this
void CompetitionCatapult::cycle() {
    if (!kill_switch.get_value()) {
        prime();
    }
    release();
    prime();
}

void CompetitionCatapult::release() {
    while (kill_switch.get_value()) {
        motors.move(cata_voltage);
    }
    stop();
}