#include "common_code/drive_builder.h"

driveClass::driveBuilder driveClass::buildDrive(drive_config_e config, pros::Controller *controller) {
    return driveBuilder(config, controller);
}

driveClass::driveBuilder::driveBuilder(drive_config_e config, pros::Controller *controller) {
    builder_obj.drive_config = config;
    builder_obj.drive_ctrler = controller;
    builder_obj.left_deadzone = 0;
    builder_obj.right_deadzone = 0;
    builder_obj.checksum = 0;
    builder_obj.left_scale = 1;
    builder_obj.right_scale = 1;
    builder_obj.sin_scale_factor = 0.8; // Unknown value
    builder_obj.accel_scale_factor = 1; // TBD
}

driveClass::driveBuilder &driveClass::driveBuilder::with_motors(pros::Motor_Group *left, pros::Motor_Group *right) {
    if (builder_obj.drive_config != TANK_c)
        throw std::runtime_error("with_motors; motor groups only allowed for tank drive config");

    builder_obj.left_drive = left;
    builder_obj.right_drive = right;
    builder_obj.checksum += 1;

    builder_obj.left_drive->set_brake_modes(BRAKETYPE_COAST);
    builder_obj.right_drive->set_brake_modes(BRAKETYPE_COAST);

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::with_motors(motor *left_1, motor *left_2, motor *right_1, motor *right_2, motor *strafe) {
    builder_obj.drive_motors.push_back(right_1);
    builder_obj.drive_motors.push_back(right_2);
    builder_obj.drive_motors.push_back(left_1);
    builder_obj.drive_motors.push_back(left_2);

    builder_obj.right_drive = new pros::Motor_Group({*right_1, *right_2}); // Used for tank drive
    builder_obj.left_drive = new pros::Motor_Group({*left_1, *left_2}); // Used for tank drive

    builder_obj.right_drive->set_brake_modes(BRAKETYPE_COAST);
    builder_obj.left_drive->set_brake_modes(BRAKETYPE_COAST);

    builder_obj.checksum += 1;

    for (int i=0; i<4; i++)
        builder_obj.drive_motors[i]->set_brake_mode(BRAKETYPE_COAST);

    if (builder_obj.drive_config == HDRIVE) {
        builder_obj.strafe_motor = strafe;
        builder_obj.strafe_motor->set_brake_mode(BRAKETYPE_COAST);
    } else {
        if (strafe != nullptr)
            throw std::runtime_error("with_motors; no strafe motor allowed for non-H-Drive config");
    }

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::with_motors(motor *left_1, motor *left_2, motor *left_3, motor *right_1, motor *right_2, motor *right_3, motor *strafe) {
    builder_obj.drive_motors.push_back(right_1);
    builder_obj.drive_motors.push_back(right_2);
    builder_obj.drive_motors.push_back(right_3);
    builder_obj.drive_motors.push_back(left_1);
    builder_obj.drive_motors.push_back(left_2);
    builder_obj.drive_motors.push_back(left_3);

    builder_obj.right_drive = new pros::Motor_Group({*right_1, *right_2, *right_3}); // Used for tank drive
    builder_obj.left_drive = new pros::Motor_Group({*left_1, *left_2, *left_3}); // Used for tank drive

    builder_obj.right_drive->set_brake_modes(BRAKETYPE_COAST);
    builder_obj.left_drive->set_brake_modes(BRAKETYPE_COAST);

    builder_obj.checksum += 1;

    for (int i=0; i<6; i++)
        builder_obj.drive_motors[i]->set_brake_mode(BRAKETYPE_COAST);

    if (builder_obj.drive_config == HDRIVE) {
        builder_obj.strafe_motor = strafe;
        builder_obj.strafe_motor->set_brake_mode(BRAKETYPE_COAST);
    } else {
        if (strafe != nullptr)
            throw std::runtime_error("with_motors; no strafe motor allowed for non-H-Drive config");
    }

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::with_motors(motor *left_1, motor *left_2, motor *left_3, motor *left_4, motor *right_1, motor *right_2, motor *right_3, motor *right_4, motor *strafe) {
    builder_obj.drive_motors.push_back(right_1);
    builder_obj.drive_motors.push_back(right_2);
    builder_obj.drive_motors.push_back(right_3);
    builder_obj.drive_motors.push_back(right_4);
    builder_obj.drive_motors.push_back(left_1);
    builder_obj.drive_motors.push_back(left_2);
    builder_obj.drive_motors.push_back(left_3);
    builder_obj.drive_motors.push_back(left_4);
    builder_obj.checksum += 1;

    builder_obj.right_drive = new pros::Motor_Group({*right_1, *right_2, *right_3, *right_4}); // Used for tank drive
    builder_obj.left_drive = new pros::Motor_Group({*left_1, *left_2, *left_3, *left_4}); // Used for tank drive

    builder_obj.right_drive->set_brake_modes(BRAKETYPE_COAST);
    builder_obj.left_drive->set_brake_modes(BRAKETYPE_COAST);

    builder_obj.checksum += 1;

    for (int i=0; i<8; i++)
        builder_obj.drive_motors[i]->set_brake_mode(BRAKETYPE_COAST);

    if (builder_obj.drive_config == HDRIVE) {
        builder_obj.strafe_motor = strafe;
        builder_obj.strafe_motor->set_brake_mode(BRAKETYPE_COAST);
    } else {
        if (strafe != nullptr)
            throw std::runtime_error("with_motors; no strafe motor allowed for non-H-Drive config");
    }

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::set_default_drive_mode(drive_mode_e mode) {
    builder_obj.checksum += 1;
    switch (builder_obj.drive_config) {
        case TANK_c:
            if (mode <= SPLIT_ARCADE_PL)
                builder_obj.drive_mode = mode;
            else
                throw std::runtime_error("set_default_drive_mode; invalid drive mode for tank drive");
            break;
        case HDRIVE || HOLONOMIC:
            if (mode >= HDRIVE_TANK && mode < CUSTOM_m)
                builder_obj.drive_mode = mode;
            else
                throw std::runtime_error("set_default_drive_mode; invalid drive mode for holonomic drive");
            break;
        case CUSTOM_c:
            builder_obj.drive_mode = mode;
            if (mode != CUSTOM_m)
                throw std::runtime_error("set_default_drive_mode; CUSTOM drive config must have CUSTOM drive mode");
            break;
        default:
            throw std::runtime_error("set_default_drive_mode; invalid drive mode");
    }

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::add_ctrl_deadzone(int left_deadzone, int right_deadzone) {
    builder_obj.left_deadzone = left_deadzone;
    builder_obj.right_deadzone = right_deadzone;

    if (left_deadzone < 0 || right_deadzone < 0)
        throw std::runtime_error("add_ctrl_deadzone; deadzone cannot be negative");
    else if (left_deadzone > 15 || right_deadzone > 15)
        throw std::runtime_error("add_ctrl_deadzone; okay c'mon jonah, get a new ctrler");

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::add_straight_drive_scale(double left_scale, double right_scale) {
    builder_obj.left_scale = left_scale;
    builder_obj.right_scale = right_scale;

    if (left_scale < 0 || right_scale < 0)
        throw std::runtime_error("straight_drive_scale; scaling factor cannot be negative");
    else if (left_scale > 1 || right_scale > 1)
        throw std::runtime_error("straight_drive_scale; scaling factor cannot be greater than 1");
    else if (left_scale < 0.8 || right_scale < 0.8)
        throw std::runtime_error("straight_drive_scale; okay, c'mon jonah, mech is lackin");

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::use_square_scaling()
{
    builder_obj.isSquareScaling = true;

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::use_sin_scaling(double sin_scale_factor)
{
    builder_obj.isSinScaling = true;
    builder_obj.sin_scale_factor = sin_scale_factor;

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::use_acceleration_scaling(double accel_scale_factor)
{
    builder_obj.isAccelScaling = true;
    builder_obj.accel_scale_factor = accel_scale_factor;

    // Add assertion for accel_scale_factor

    return *this;
}

driveClass driveClass::driveBuilder::init() {
    if (builder_obj.checksum != 2)
        throw std::runtime_error("init; incorrect drive construction parameters");

    return builder_obj;
}

void driveClass::tank() {
    switch (drive_mode) {
        case TANK_m:
            // if (isSquareScaling) {
            //     left *= square_scale(normalize_joystick(drive_ctrler->get_analog(LEFT_Y_AXIS)));  
            //     right *= square_scale(normalize_joystick(drive_ctrler->get_analog(RIGHT_Y_AXIS)));
            // }            
            break;
        case SINGLE_STICK_ARCADE_R:
            
            break;
        case SINGLE_STICK_ARCADE_L:
            
            break;
        case SPLIT_ARCADE_PR:
            
            break;
        case SPLIT_ARCADE_PL:
            
            break;
        case CUSTOM_m:
            // Add custom drive controls here
            break;
        default:
            throw std::runtime_error("tank; error in drive mode");
    }
}

void driveClass::hdrive() {

}

void driveClass::holonomic_drive() {

}

void driveClass::xdrive() {

}

void driveClass::custom_drive() {

}

// void driveClass::tank_drive() {

// }

// void driveClass::single_stick_arcade_right() {

// }

// void driveClass::single_stick_arcade_left() {

// }

// void driveClass::split_arcade_right() {

// }

// void driveClass::split_arcade_left() {

// }

// void driveClass::custom_drive_mode() {

// }

double driveClass::square_scale(double input) {
    return copysign(pow(input, 2), input);
}

double driveClass::sin_scale(double input) {
    return copysign(pow(sin((3.14159/2) * fabs(input)), sin_scale_factor), input);
}

double driveClass::accel_scale(double input) {
    // TBD
    throw std::runtime_error("accel_scale; not implemented");
}

double driveClass::normalize_joystick(double input) {
    return input / 127.0;
}

void driveClass::start_drive() {
    
    while(1) {
        tank();
        delay(20);
    }
}

void driveClass::pause_drive() {

}

void driveClass::stop_drive() {

}

void driveClass::test_drive() {

}

void driveClass::set_drive_mode(drive_mode_e mode) {
    switch (drive_config) {
        case TANK_c:
            if (mode <= SPLIT_ARCADE_PL)
                drive_mode = mode;
            else
                throw std::runtime_error("set_drive_mode; invalid drive mode for tank drive");
            break;
        case HDRIVE || HOLONOMIC:
            if (mode >= HDRIVE_TANK && mode < CUSTOM_m)
                drive_mode = mode;
            else
                throw std::runtime_error("set_drive_mode; invalid drive mode for holonomic drive");
            break;
        case CUSTOM_c:
            drive_mode = mode;
            if (mode != CUSTOM_m)
                throw std::runtime_error("set_drive_mode; CUSTOM drive config must have CUSTOM drive mode");
            break;
        default:
            throw std::runtime_error("set_drive_mode; invalid drive mode");
    }
}