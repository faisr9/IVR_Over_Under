#include "comp_15/competition_catapult.h"

// note: velocity in rpm
extern CompetitionCatapult* CompetitionCatapult::instance_ = nullptr;

CompetitionCatapult* CompetitionCatapult::createInstance(pros::Motor& motor1, pros::Motor& motor2, pros::ADIDigitalIn& killswitch, int load, int launch) {
    if (!instance_) {
        instance_ = new CompetitionCatapult(motor1, motor2, killswitch, load, launch);
    }
    return instance_;
}
CompetitionCatapult::CompetitionCatapult(pros::Motor& motor1, pros::Motor& motor2, pros::ADIDigitalIn& killswitch, int load, int launch) 
    : SubsystemParent("Competition Catapult"), left_motor(motor1), right_motor(motor2), kill_switch(killswitch),
        left_start(motor1.get_position()), right_start(motor2.get_position()), load_voltage(load), launch_voltage(launch) {
    
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
    left_motor.move_voltage(0);
    right_motor.move_voltage(0);
}

void CompetitionCatapult::set_power(int power) {
    launch_voltage = power;
}

void CompetitionCatapult::starting_position() {
    left_motor.move_absolute(left_start, 10);
    right_motor.move_absolute(right_start, 10);
}
void CompetitionCatapult::load() {
    while (true) {
        if (kill_switch.get_value()) {
            stop();
            break;
        }
        left_motor.move_voltage(-load_voltage);
        right_motor.move_voltage(-load_voltage);
        pros::delay(20);
    }
}
void CompetitionCatapult::launch() {
    left_motor.move_voltage(launch_voltage);
    right_motor.move_voltage(launch_voltage);
}