#include "comp_18/comp18_includeList.h"

bool Climb::activate() {
    while(motor1.get_position() < max_position && motor2.get_position() < max_position) {  
        motor1.move_velocity(velocity);
        motor2.move_velocity(velocity);
    }

    return true;
}

Climb::Climb(pros::Motor& m1, pros::Motor& m2, double v, double mp) : motor1(m1), motor2(m2), max_position(mp), SubsystemParent("Climb") {
    if (v < 0) {
        throw std::invalid_argument("negative velocity");
    }

    velocity = v;
}

Climb* Climb::createInstance(pros::Motor& m1, pros::Motor& m2, double v, double mp) {
    if (!instance_) {
        instance_ = new Climb(m1, m2, v, mp);
    }

    return instance_;
}

Climb* Climb::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

Climb::~Climb() {
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}
void Climb::stop() {
}