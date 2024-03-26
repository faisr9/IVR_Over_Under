#include "common_code/drive_builder.h"

drive_builder::drive_builder(drive_config_e config, pros::Controller *controller) {
    drive_config = config;
    drive_controller = controller;
    left_deadzone = 0;
    right_deadzone = 0;
    checksum = 0;
}

drive_builder &drive_builder::with_motors(pros::Motor_Group *left, pros::Motor_Group *right) {
    if (drive_config != TANK)
        throw std::runtime_error("with_motors; drive_config is not TANK");

    left_drive = left;
    right_drive = right;
    checksum = 1;
}

drive_builder &drive_builder::with_motors(motor *left_1, motor *left_2, motor *right_1, motor *right_2, motor *strafe) {
    drive_motors[0] = right_1;
    drive_motors[1] = right_2;
    drive_motors[2] = left_1;
    drive_motors[3] = left_2;

    for (int i=0; i<4; i++)
        drive_motors[i]->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    if (drive_config == HDRIVE) {
        strafe_motor = strafe;
        strafe_motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    } else {
        if (strafe != nullptr)
            throw std::runtime_error("with_motors; no strafe motor for non-H-Drive config");
    }
}

drive_builder &drive_builder::with_motors(motor *left_1, motor *left_2, motor *left_3, motor *right_1, motor *right_2, motor *right_3, motor *strafe) {
    drive_motors[0] = right_1;
    drive_motors[1] = right_2;
    drive_motors[2] = right_3;
    drive_motors[3] = left_1;
    drive_motors[4] = left_2;
    drive_motors[5] = left_3;

    for (int i=0; i<6; i++)
        drive_motors[i]->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    if (drive_config == HDRIVE) {
        strafe_motor = strafe;
        strafe_motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    } else {
        if (strafe != nullptr)
            throw std::runtime_error("with_motors; no strafe motor for non-H-Drive config");
    }
}

drive_builder &drive_builder::with_motors(motor *left_1, motor *left_2, motor *left_3, motor *left_4, motor *right_1, motor *right_2, motor *right_3, motor *right_4, motor *strafe) {
    drive_motors[0] = right_1;
    drive_motors[1] = right_2;
    drive_motors[2] = right_3;
    drive_motors[3] = right_4;
    drive_motors[4] = left_1;
    drive_motors[5] = left_2;
    drive_motors[6] = left_3;
    drive_motors[7] = left_4;

    for (int i=0; i<8; i++)
        drive_motors[i]->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    if (drive_config == HDRIVE) {
        strafe_motor = strafe;
        strafe_motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    } else {
        if (strafe != nullptr)
            throw std::runtime_error("with_motors; no strafe motor for non-H-Drive config");
    }
}

drive_builder &drive_builder::set_default_drive_mode(drive_mode_e mode) {
    switch (drive_config) {
        case TANK:
            if (mode <= SPLIT_ARCADE_PL)
                drive_mode = mode;
            else
                throw std::runtime_error("set_default_drive_mode; invalid drive mode for tank drive");
            break;
        case HDRIVE || HOLONOMIC:
            if (mode >= HDRIVE_TANK && mode < CUSTOM)
                drive_mode = mode;
            else
                throw std::runtime_error("set_default_drive_mode; invalid drive mode for holonomic drive");
            break;
        case CUSTOM:
            drive_mode = mode;
            break;
        default:
            drive_mode = TANK;
    }
}

drive_builder &drive_builder::add_ctrl_deadzone(int left_deadzone, int right_deadzone) {
    this->left_deadzone = left_deadzone;
    this->right_deadzone = right_deadzone; 
}

drive_builder &drive_builder::init() {

}

void drive_builder::tank() {

}

void drive_builder::hdrive() {

}

void drive_builder::holonomic_drive() {

}

void drive_builder::xdrive() {

}

void drive_builder::custom_drive() {

}

void drive_builder::tank_drive() {

}

void drive_builder::single_stick_arcade_right() {

}

void drive_builder::single_stick_arcade_left() {

}

void drive_builder::split_arcade_right() {

}

void drive_builder::split_arcade_left() {

}

void drive_builder::custom_drive_mode() {

}

void drive_builder::square_scale(double input) {

}

void drive_builder::sin_scale(double input, double sin_scale_factor) {

}

void drive_builder::start_drive() {

}

void drive_builder::pause_drive() {

}

void drive_builder::stop_drive() {

}

void drive_builder::test_drive() {

}
