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
    cata_mode = "X";
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
void SkillsCata::cycle(){
    motors.move_velocity(cata_rpm);
    current_rot = 0; // set to 0 so we have a defined initial value for last_rot

    // stop when the value jumps from 36000 to 0
    while (current_rot >= last_rot) {
        last_rot = current_rot;
        current_rot = rotation_sensor.get_angle() - upright_position;
        if (current_rot < 0) current_rot += 36000; // centidegrees

        pros::delay(50);
    }
    // want to stop when jump from 36000 -> 0

    stop();
}


std::string SkillsCata::get_cata_mode() {
    return cata_mode;
}


// for use by everything but the actual catapult task
void SkillsCata::set_cata_mode(std::string new_cata_mode) {
    cata_task.suspend();
    cata_mode = new_cata_mode;
    cata_task.resume();
}


// for use by cata_task_funct so it does not suspend itself
void SkillsCata::set_cata_mode_internal(std::string new_cata_mode) {
    cata_mode = new_cata_mode;
}


// Function for the catapult class' task. Don't call this unless you're passing it to a task
void cata_task_funct() {
    SkillsCata* cata_inst = SkillsCata::getInstance();

    while (!cata_inst) {
        pros::delay(3000);
        cata_inst = SkillsCata::getInstance();
    }

    while (1) {
        std::string cata_mode = cata_inst->get_cata_mode();

        if (cata_mode == "X") {
            cata_inst->stop();
        } else if (cata_mode == "C") {
            cata_inst->cycle();
            cata_inst->set_cata_mode_internal("X");
        } else {
            pros::lcd::set_text(6, "Invalid cata_mode string!");
            cata_inst->stop();
        }

        pros::delay(30);
    }
}