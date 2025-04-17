#include "skills_thrower/cata.h"

SkillsCata* SkillsCata::instance_ = nullptr;

SkillsCata* SkillsCata::createInstance(pros::MotorGroup& motorgroup, pros::Rotation& rotation_sensor){
    if (!instance_) {
        instance_ = new SkillsCata(motorgroup, rotation_sensor);
    }

    return instance_;
}


SkillsCata* SkillsCata::getInstance(){
    if (instance_ == nullptr) {
        throw std::runtime_error("Cata: Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}


SkillsCata::SkillsCata(pros::MotorGroup& motorgroup, pros::Rotation& rotation_sensor) : SubsystemParent("Skills_Cata"), motors(motorgroup), rotation_sensor(rotation_sensor), cata_task(cata_task_funct) {
    cata_mode = CataMode::Idle;
    motors.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
}

 
SkillsCata::~SkillsCata(){
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}


void SkillsCata::stop(){
    motors.brake();
}


// blocking function. Assumes the rotation value increases as the catapult goes towards firing
void SkillsCata::cycle(bool stop_at_end){
    // TODO: Update because the rot sensor loc is on the gear not motor, gear goes BACK AND FORTH not just one dir
    // motors still only go one direction though

    motors.move_velocity(abs(cata_rpm)); // abs just to avoid disaster
    double current_rot = rotation_sensor.get_angle(); // set to 0 so we have a defined initial value for last_rot
    double last_rot = current_rot - 1;
    int cycles_backwards = 0;

    // stop when we go backwards for 2 cycles in a row
    while (cycles_backwards < 2) {
        current_rot = rotation_sensor.get_angle();

        if (current_rot > last_rot) { // not = bc if told to stop externally we want it to exit from here
            cycles_backwards++;
        } else {
            cycles_backwards = 0;
        }

        last_rot = current_rot;

        pros::delay(20);
    }
    // want to stop when jump from 36000 -> 0

    if (stop_at_end) {
        stop();
    }
}


SkillsCata::CataMode SkillsCata::get_cata_mode() {
    return cata_mode;
}


// for use by everything but the actual catapult task
void SkillsCata::set_cata_mode(CataMode new_cata_mode) {
    cata_task.suspend();
    cata_mode = new_cata_mode;
    cata_task.resume();
}


// for use by cata_task_funct so it does not suspend itself
void SkillsCata::set_cata_mode_internal(CataMode new_cata_mode) {
    cata_mode = new_cata_mode;
}

void SkillsCata::move_forward_manual() {
    cata_motors.move_velocity(abs(cata_rpm));
}


// Function for the catapult class' task. Don't call this unless you're passing it to a task
void cata_task_funct() {
    SkillsCata* cata_inst = SkillsCata::getInstance();

    while (!cata_inst) {
        pros::delay(3000);
        cata_inst = SkillsCata::getInstance();
    }

    while (1) {
        SkillsCata::CataMode cata_mode = cata_inst->get_cata_mode();

        if (cata_mode == SkillsCata::CataMode::Idle) {
            // do nothing
        } else if (cata_mode == SkillsCata::CataMode::Cycle) {
            cata_inst->cycle(true);
            cata_inst->set_cata_mode_internal(SkillsCata::CataMode::Idle);
        } else {
            pros::lcd::set_text(6, "Invalid cata_mode string!");
        }

        pros::delay(30);
    }
}