#include "comp_15/cata.h"

CompetitionCatapult* CompetitionCatapult::createInstance(pros::MotorGroup& motorgroup, pros::ADIButton& limit_switch) {
    if (!instance_) {
        instance_ = new CompetitionCatapult(motorgroup, limit_switch);
    }
    return instance_;
}

CompetitionCatapult::CompetitionCatapult(pros::MotorGroup& motorgroup, pros::ADIButton& limit_switch) 
    : SubsystemParent("Competition Catapult"), motors(motorgroup), kill_switch(limit_switch), cata_task(cata_task_funct){

    cata_mode = "X";
    cata_move_power = 127;
    motors.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
}

CompetitionCatapult* CompetitionCatapult::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}
CompetitionCatapult::~CompetitionCatapult() {
    cata_task.suspend();
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

void CompetitionCatapult::stop() {
    motors.move(0);
}

void CompetitionCatapult::move_forward_manual() {
    motors.move(cata_move_power);
}

void CompetitionCatapult::prime() {
    while (!kill_switch.get_value()) {
        motors.move(cata_move_power);
		pros::delay(30);
	}

    stop();
}

void CompetitionCatapult::cycle() {
    if (!kill_switch.get_value()) {
        prime();
        release();
    } else {
        release();
        prime();
    }
}

bool CompetitionCatapult::get_kill_switch_on(){
    return kill_switch.get_value();
}


void CompetitionCatapult::release() {
    while (kill_switch.get_value()) {
        motors.move(cata_move_power);
        pros::delay(30);
    }
    stop();
}

std::string CompetitionCatapult::get_cata_mode() {
    return cata_mode;
}

// for use by everything but the actual catapult task
void CompetitionCatapult::set_cata_mode(std::string new_cata_mode) {
    cata_task.suspend();
    cata_mode = new_cata_mode;
    cata_task.resume();
}

// for use by cata_task_funct so it does not suspend itself
void CompetitionCatapult::set_cata_mode_internal(std::string new_cata_mode) {
    cata_mode = new_cata_mode;
}


// Function for the catapult class' task. Don't call this unless you're passing it to a task
void cata_task_funct() {
    CompetitionCatapult* cata_inst = CompetitionCatapult::getInstance();

    while (!cata_inst) {
        pros::delay(3000);
        cata_inst = CompetitionCatapult::getInstance();
    }

    while (1) {
        std::string cata_mode = cata_inst->get_cata_mode();

        if (cata_mode == "X") {
            cata_inst->stop();
        } else if (cata_mode == "PR") {
            cata_inst->prime();
            cata_inst->set_cata_mode_internal("R");
        } else if (cata_mode == "RP") {
            cata_inst->release();
            cata_inst->set_cata_mode_internal("P");
        } else if (cata_mode == "R") {
            cata_inst->release();
            cata_inst->set_cata_mode_internal("X");
        } else if (cata_mode == "P") {
            cata_inst->prime();
            cata_inst->set_cata_mode_internal("X");
        } else if (cata_mode == "I") {
            cata_inst->move_forward_manual();
            cata_inst->set_cata_mode_internal("X");
        } else {
            pros::lcd::set_text(6, "Invalid cata_mode string!");
            cata_inst->stop();
        }

        pros::delay(30);
    }
}