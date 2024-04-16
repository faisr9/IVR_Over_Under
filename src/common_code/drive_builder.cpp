#include "common_code/drive_builder.h"

driveClass *driveClass::drive_instance_ = nullptr;

driveClass::driveBuilder driveClass::buildDrive(drive_config_e config, pros::Controller *controller) {
    return driveBuilder(config, controller);
}

driveClass::driveBuilder::driveBuilder(drive_config_e config, pros::Controller *controller) {
    builder_obj.drive_config = config;
    builder_obj.drive_ctrler = controller;
    builder_obj.left_deadzone = 0;
    builder_obj.right_deadzone = 0;
    builder_obj.checksum = 0x10; // 0001 0000
    builder_obj.left_scale = 1;
    builder_obj.right_scale = 1;
    builder_obj.sin_scale_factor = 0.8; // Unknown value
    builder_obj.isSquareScaling = false;
    builder_obj.isSinScaling = false;
    builder_obj.runDriveLoop = true;
}

driveClass::driveBuilder &driveClass::driveBuilder::with_motors(pros::Motor_Group *left, pros::Motor_Group *right) {
    if (builder_obj.drive_config != TANK_c)
        throw std::runtime_error("with_motors; motor groups only allowed for tank drive config");

    builder_obj.left_drive = left;
    builder_obj.right_drive = right;
    builder_obj.checksum <<= 1; // 0010 0000

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

    builder_obj.checksum <<= 1; // 0010 0000

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

    builder_obj.checksum <<= 1; // 0010 0000

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

    builder_obj.right_drive = new pros::Motor_Group({*right_1, *right_2, *right_3, *right_4}); // Used for tank drive
    builder_obj.left_drive = new pros::Motor_Group({*left_1, *left_2, *left_3, *left_4}); // Used for tank drive

    builder_obj.right_drive->set_brake_modes(BRAKETYPE_COAST);
    builder_obj.left_drive->set_brake_modes(BRAKETYPE_COAST);

    builder_obj.checksum <<= 1; // 0010 0000

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
    builder_obj.checksum |= 0x08; // 0010 1000
    switch (builder_obj.drive_config) {
        case TANK_c:
            if (mode <= SPLIT_ARCADE_PL)
                builder_obj.drive_mode = mode;
            else
                throw std::runtime_error("set_default_drive_mode; invalid drive mode for tank drive");
            break;
        case HDRIVE:
            if (mode >= HDRIVE_TANK && mode < CUSTOM_m)
                builder_obj.drive_mode = mode;
            else
                throw std::runtime_error("set_default_drive_mode; invalid drive mode for holonomic drive");
            break;
        case HOLONOMIC:
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
    builder_obj.checksum |= 0x04; // 0010 1100

    return *this;
}

driveClass::driveBuilder &driveClass::driveBuilder::use_sin_scaling(double sin_scale_factor)
{
    builder_obj.isSinScaling = true;
    builder_obj.sin_scale_factor = sin_scale_factor;
    builder_obj.checksum |= 0x02; // 0010 1010

    // Add assertion for sin_scale_factor

    return *this;
}

driveClass *driveClass::driveBuilder::init() {
    // Correct checksums: 0010 1000(0x28), 0010 1100(0x2C), 0010 1010(0x2A)
    if (builder_obj.checksum == 0x28 || 
        builder_obj.checksum == 0x2C || 
        builder_obj.checksum == 0x2A)
    {
        driveClass::drive_instance_ = &builder_obj;
        return drive_instance_;
    }
    else    
        throw std::runtime_error("init; bad drive builder config");    
}

void driveClass::tank() {
    int value = 1; // idk why, but this works
    runDriveLoop = true;
    while(runDriveLoop) {
        cout << "IM HERE" << endl;
        // updateAxis();

        if (value == (int)TANK_m) {
            drive_motors[0]->move_velocity(drive_ctrler->get_analog(LEFT_Y_AXIS));
            left_drive->move_voltage(1111); // throwing prefetch error
            // something wrong with pointers to motors
            // left_drive->move_voltage(motorPower * calc_axis.l_y);
            // right_drive->move_voltage(motorPower * calc_axis.r_y);
        } else if (value == (int)SINGLE_STICK_ARCADE_R) {
            
        } else if (value == (int)SINGLE_STICK_ARCADE_L) {
            
        } else if (value == (int)SPLIT_ARCADE_PR) {
            
        } else if (value == (int)SPLIT_ARCADE_PL) {
            
        } else if (value == (int)CUSTOM_m) {
            // Add custom drive controls here
        } else {
            throw std::runtime_error("tank; error in drive mode");
        }
        delay(15);
    }
}

void driveClass::hdrive() {

}

void driveClass::holonomic_drive() {

}

// void driveClass::xdrive() {

// }

void driveClass::custom_config() {

}

// void driveClass::setMotors() {

// }

void driveClass::updateAxis() {
    // Get axis values
    raw_axis.l_x = drive_ctrler->get_analog(LEFT_X_AXIS);
    raw_axis.l_y = drive_ctrler->get_analog(LEFT_Y_AXIS);
    raw_axis.r_x = drive_ctrler->get_analog(RIGHT_X_AXIS);
    raw_axis.r_y = drive_ctrler->get_analog(RIGHT_Y_AXIS);

    // Apply dead zones
    raw_axis.l_x = raw_axis.l_x > left_deadzone || raw_axis.l_x < -left_deadzone ? raw_axis.l_x : 0;
    raw_axis.l_y = raw_axis.l_y > left_deadzone || raw_axis.l_y < -left_deadzone ? raw_axis.l_y : 0;
    raw_axis.r_x = raw_axis.r_x > right_deadzone || raw_axis.r_x < -right_deadzone ? raw_axis.r_x : 0;
    raw_axis.r_y = raw_axis.r_y > right_deadzone || raw_axis.r_y < -right_deadzone ? raw_axis.r_y : 0;

    // Apply scaling factors
    if (isSquareScaling + isSinScaling > 1)
        throw std::runtime_error("tank_drive; multiple scaling factors enabled");
    else {
        if (isSquareScaling) {
            calc_axis.l_x = square_scale(normalize_joystick(raw_axis.l_x));
            calc_axis.l_y = square_scale(normalize_joystick(raw_axis.l_y));
            calc_axis.r_x = square_scale(normalize_joystick(raw_axis.r_x));
            calc_axis.r_y = square_scale(normalize_joystick(raw_axis.r_y));
        } else if (isSinScaling) {
            calc_axis.l_x = sin_scale(normalize_joystick(raw_axis.l_x));
            calc_axis.l_y = sin_scale(normalize_joystick(raw_axis.l_y));
            calc_axis.r_x = sin_scale(normalize_joystick(raw_axis.r_x));
            calc_axis.r_y = sin_scale(normalize_joystick(raw_axis.r_y));
        } else {
            calc_axis.l_x = normalize_joystick(raw_axis.l_x);
            calc_axis.l_y = normalize_joystick(raw_axis.l_y);
            calc_axis.r_x = normalize_joystick(raw_axis.r_x);
            calc_axis.r_y = normalize_joystick(raw_axis.r_y);
        }
    }

    // pros::Task lcd_task([=] {
		// while(true) {
			pros::lcd::print(0, "r_x: %d", calc_axis.r_x);
			pros::lcd::print(1, "r_y: %d", calc_axis.r_y);
			pros::lcd::print(2, "l_x: %d", calc_axis.l_x);
			pros::lcd::print(3, "l_y: %d", calc_axis.l_y);
			pros::delay(50);
		// }
	// });
}

// driveClass::ctrler_axis_s driveClass::tank_drive() {
    
// }

// driveClass::ctrler_axis_s driveClass::single_stick_arcade_right() {

    
// }

// driveClass::ctrler_axis_s driveClass::single_stick_arcade_left() {

    
// }

// driveClass::ctrler_axis_s driveClass::split_arcade_right() {

    
// }

// driveClass::ctrler_axis_s driveClass::split_arcade_left() {

    
// }

// driveClass::ctrler_axis_s driveClass::custom_mode() {

    
// }

double driveClass::square_scale(double input) {
    return copysign(pow(input, 2), input);
}

double driveClass::sin_scale(double input) {
    return copysign(pow(sin((3.14159/2) * fabs(input)), sin_scale_factor), input);
}

double driveClass::normalize_joystick(double input) {
    return input / 127.0;
}

void driveClass::start_drive() {
    if (runDriveLoop) {
        // left = right = strafe = turn = fwd = 12000; // Set to max voltage
        // stop_drive(); // When changing drive modes, allows for the previous drive mode to stop

        switch (drive_config) {
            case TANK_c:
                tank_task = new pros::Task([=] { this->drive_instance_->tank(); });
                // tank_task->set_priority(TASK_PRIORITY_MEDIUM_HIGH);
                break;
            case HDRIVE:
                hDrive_task = new pros::Task([=] { this->drive_instance_->hdrive(); });
                // hDrive_task->set_priority(TASK_PRIORITY_MEDIUM_HIGH);
                break;
            case HOLONOMIC:
                holonomic_task = new pros::Task([=] { this->drive_instance_->holonomic_drive(); });
                // holonomic_task->set_priority(TASK_PRIORITY_MEDIUM_HIGH);
                break;
            case CUSTOM_c:
                custom_driveConf_task = new pros::Task([=] { this->drive_instance_->custom_config(); });
                // custom_driveConf_task->set_priority(TASK_PRIORITY_MEDIUM_HIGH);
                break;
            default:
                throw std::runtime_error("start_drive-init; invalid drive config");
        }
    } else {
        switch (drive_config) {
            case TANK_c:
                tank_task->resume();
                break;
            case HDRIVE:
                hDrive_task->resume();
                break;
            case HOLONOMIC:
                holonomic_task->resume();
                break;
            case CUSTOM_c:
                custom_driveConf_task->resume();
                break;
            default:
                throw std::runtime_error("start_drive-resume; invalid drive config");
        }
    }
}

void driveClass::pause_drive() {
    runDriveLoop = false;
}

void driveClass::stop_drive() {
    // Used for when drive mode is changed. Stops all drive tasks
    if (tank_task->get_state() == E_TASK_STATE_RUNNING || tank_task->get_state() == E_TASK_STATE_SUSPENDED)
        tank_task->remove();
    if (hDrive_task->get_state() == E_TASK_STATE_RUNNING || hDrive_task->get_state() == E_TASK_STATE_SUSPENDED)
        hDrive_task->remove();
    if (holonomic_task->get_state() == E_TASK_STATE_RUNNING || holonomic_task->get_state() == E_TASK_STATE_SUSPENDED)
        holonomic_task->remove();
    if (custom_driveConf_task->get_state() == E_TASK_STATE_RUNNING || custom_driveConf_task->get_state() == E_TASK_STATE_SUSPENDED)
        custom_driveConf_task->remove();
}

void driveClass::test_drive() {

}

void driveClass::set_drive_mode(drive_mode_e mode, bool resume) {
    switch (drive_config) {
        case TANK_c:
            if (mode <= SPLIT_ARCADE_PL)
                drive_mode = mode;
            else
                throw std::runtime_error("set_drive_mode; invalid drive mode for tank drive");
            break;
        case HDRIVE:
            if (mode >= HDRIVE_TANK && mode < CUSTOM_m)
                drive_mode = mode;
            else
                throw std::runtime_error("set_drive_mode; invalid drive mode for holonomic drive");
            break;
        case HOLONOMIC:
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
    // Makes task restart with new drive mode
    start_drive();
    if(!resume)
        pause_drive();
}