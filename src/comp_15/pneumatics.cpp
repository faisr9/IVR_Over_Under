#include "comp_15/pneumatics.h"

Pneumatics* Pneumatics::instance_ = nullptr;

Pneumatics* Pneumatics::createInstance(char left_wing_port, char right_wing_port, char intake_port) {
    if (!instance_) {
        instance_ = new Pneumatics(left_wing_port, right_wing_port, intake_port);
    }

    return instance_;
}

Pneumatics* Pneumatics::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

Pneumatics::Pneumatics(char left_wing_port, char right_wing_port, char intake_port) : SubsystemParent("Pneumatics"), left_wing_(left_wing_port), right_wing_(right_wing_port), intake_(intake_port) {}

Pneumatics::~Pneumatics() {
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

Piston& Pneumatics::getLeftWing() {
    return left_wing_;
}

Piston& Pneumatics::getRightWing() {
    return right_wing_;
}

Piston& Pneumatics::getIntake() {
    return intake_;
}

void Pneumatics::stop() {
    left_wing_.off();
    right_wing_.off();
    intake_.off();
}
