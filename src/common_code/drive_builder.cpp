#include "common_code/drive_builder.h"

drive_builder::drive_builder(drive_config_e config, pros::Controller *controller) {
    drive_config = config;
    drive_controller = controller;
    left_deadzone = 0;
    right_deadzone = 0;
    checksum = 0;
    left_scale = 1;
    right_scale = 1;
    sin_scale_factor = 0.8; // Unknown value
    accel_scale_factor = 1; // TBD
}

drive_builder &drive_builder::with_motors(pros::Motor_Group *left, pros::Motor_Group *right) {
    if (drive_config != TANK)
        throw std::runtime_error("with_motors; drive_config is not TANK");

    left_drive = left;
    right_drive = right;
    checksum += 1;

    return *this;
}

drive_builder &drive_builder::with_motors(motor *left_1, motor *left_2, motor *right_1, motor *right_2, motor *strafe) {
    drive_motors[0] = right_1;
    drive_motors[1] = right_2;
    drive_motors[2] = left_1;
    drive_motors[3] = left_2;

    right_drive = new pros::Motor_Group({*right_1, *right_2}); // Used for tank drive
    left_drive = new pros::Motor_Group({*left_1, *left_2}); // Used for tank drive

    checksum += 1;

    for (int i=0; i<4; i++)
        drive_motors[i]->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    if (drive_config == HDRIVE) {
        strafe_motor = strafe;
        strafe_motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    } else {
        if (strafe != nullptr)
            throw std::runtime_error("with_motors; no strafe motor allowed for non-H-Drive config");
    }

    return *this;
}

drive_builder &drive_builder::with_motors(motor *left_1, motor *left_2, motor *left_3, motor *right_1, motor *right_2, motor *right_3, motor *strafe) {
    drive_motors[0] = right_1;
    drive_motors[1] = right_2;
    drive_motors[2] = right_3;
    drive_motors[3] = left_1;
    drive_motors[4] = left_2;
    drive_motors[5] = left_3;

    right_drive = new pros::Motor_Group({*right_1, *right_2, *right_3}); // Used for tank drive
    left_drive = new pros::Motor_Group({*left_1, *left_2, *left_3}); // Used for tank drive

    checksum += 1;

    for (int i=0; i<6; i++)
        drive_motors[i]->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    if (drive_config == HDRIVE) {
        strafe_motor = strafe;
        strafe_motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    } else {
        if (strafe != nullptr)
            throw std::runtime_error("with_motors; no strafe motor allowed for non-H-Drive config");
    }

    return *this;
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
    checksum += 1;

    right_drive = new pros::Motor_Group({*right_1, *right_2, *right_3, *right_4}); // Used for tank drive
    left_drive = new pros::Motor_Group({*left_1, *left_2, *left_3, *left_4}); // Used for tank drive

    for (int i=0; i<8; i++)
        drive_motors[i]->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    if (drive_config == HDRIVE) {
        strafe_motor = strafe;
        strafe_motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    } else {
        if (strafe != nullptr)
            throw std::runtime_error("with_motors; no strafe motor allowed for non-H-Drive config");
    }

    return *this;
}

drive_builder &drive_builder::set_default_drive_mode(drive_mode_e mode) {
    checksum += 1;
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
            if (mode != CUSTOM)
                throw std::runtime_error("set_default_drive_mode; CUSTOM drive config must have CUSTOM drive mode");
            break;
        default:
            drive_mode = drive_mode_e::TANK;
    }

    return *this;
}

drive_builder &drive_builder::add_ctrl_deadzone(int left_deadzone, int right_deadzone) {
    this->left_deadzone = left_deadzone;
    this->right_deadzone = right_deadzone;

    return *this;
}

drive_builder &drive_builder::add_straight_drive_scale(double left_scale, double right_scale) {
    this->left_scale = left_scale;
    this->right_scale = right_scale;

    if (left_scale < 0 || right_scale < 0)
        throw std::runtime_error("straight_drive_scale; scaling factor cannot be negative");
    else if (left_scale > 1 || right_scale > 1)
        throw std::runtime_error("straight_drive_scale; scaling factor cannot be greater than 1");
    else if (left_scale < 0.8 || right_scale < 0.8)
        throw std::runtime_error("straight_drive_scale; okay, c'mon jonah, mech is lackin");

    return *this;
}

drive_builder &drive_builder::use_square_scaling()
{
    isSquareScaling = true;

    return *this;
}

drive_builder &drive_builder::use_sin_scaling(double sin_scale_factor)
{
    isSinScaling = true;
    this->sin_scale_factor = sin_scale_factor;

    return *this;
}

drive_builder &drive_builder::use_acceleration_scaling(double accel_scale_factor)
{
    isAccelScaling = true;
    this->accel_scale_factor = accel_scale_factor;

    return *this;
}


drive_builder &drive_builder::init() {
    if (checksum != 2)
        throw std::runtime_error("init; incorrect drive configuration");

    return *this;
}

void driveClass::tank() {

}

void driveClass::hdrive() {

}

void driveClass::holonomic_drive() {

}

void driveClass::xdrive() {

}

void driveClass::custom_drive() {

}

void driveClass::tank_drive() {

}

void driveClass::single_stick_arcade_right() {

}

void driveClass::single_stick_arcade_left() {

}

void driveClass::split_arcade_right() {

}

void driveClass::split_arcade_left() {

}

void driveClass::custom_drive_mode() {

}

void driveClass::square_scale(double input) {

}

void driveClass::sin_scale(double input, double sin_scale_factor) {

}

void driveClass::start_drive() {

}

void driveClass::pause_drive() {

}

void driveClass::stop_drive() {

}

void driveClass::test_drive() {

}
