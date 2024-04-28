/*
 * Description: implementations for traditional drive system
 * Path: src/common_code/traditional_drive.cpp
 * Header: include/common_code/traditional_drive.h
 * Last Modified: 04/14/24 by Zach Martin
 */

#include "common_code/traditional_drive.h"

// most basic version that initializes all required variables
traditional_drive::traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, int m)
    : DriveParent(imu, drive_mode[m]) 
{
    init(imu, l, r, m);
};

// initialize controller if applicable
traditional_drive::traditional_drive(Imu &imu, Controller &mstr, Motor_Group &l, Motor_Group &r, int mode) 
    : DriveParent(imu, drive_mode[mode])
{
    master = &mstr;
    init(imu, l, r, mode);
}

// with odometry
traditional_drive::traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, Odom& odometry) 
    : DriveParent(imu, drive_mode[0]) 
{
    odom_inst_ = &odometry;
    init(imu, l, r, 0);
} 

// with odom and controller
traditional_drive::traditional_drive(Imu&imu, Controller &mstr, Motor_Group &l, Motor_Group &r, Odom& odometry) 
    : DriveParent(imu, drive_mode[0]) 
{
    master = &mstr;
    odom_inst_ = &odometry; 
    init(imu, l, r, 0);
}

// initialize variables
void traditional_drive::init(Imu &imu, Motor_Group &l, Motor_Group &r, int mode) 
{
    // set controller and motor groups
    imu_=&imu;
    left_side = &l;
    right_side = &r;
    mode_ = mode;

    auto gearing = l[0].get_gearing(); // assume all motors have the same gearing

    // set max speed based on gear
    if (gearing == 0)      // 36:1
        maxspeed = 100.0;  // max rpm
    else if (gearing == 1) // 18:1
        maxspeed = 200.0;  // max rpm
    else if (gearing == 2) // 6:1
        maxspeed = 600.0;  // max rpm
    else
        maxspeed = 200.0; // default max rpm
};

// ************ destructor ************
traditional_drive::~traditional_drive()
{
    if (!odom_inst_) {
        delete odom_inst_;
        odom_inst_ = nullptr;
    }
    // turn off motors
    stop();

    // set pointers to null to avoid dangling pointers
    imu_ = nullptr;
    master = nullptr;
    left_side = nullptr;
    right_side = nullptr;
};

void traditional_drive::change_drive_mode(int mode) 
{
    this->mode_ = mode;
}

// toggle drive mode (arcade, tank, hybrid)
void traditional_drive::toggle_drive_mode()
{
    // 0 = arcade, 1 = tank, 2 = hybrid
    switch (mode_)
    {
    case 0:
        arcade_drive(); // call arcade drive
        break;
    case 1:
        tank_drive(); // call tank drive
        break;
    case 2:
        hybrid_drive(); // call hybrid drive
        break;
    default:
        stop(); // stop motors
        break;
    }
};

// operator control tank drive
void traditional_drive::tank_drive()
{
    // get joystick values and apply square scaling
    left *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)), 2.25);   // vertical input from left joystick
    right *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)), 2.25); // vertical input from right joystick

    setV(); // set voltage to motors
};

// operator control hybrid drive
void traditional_drive::hybrid_drive()
{
    // get joystick values and apply square scaling
    fwd = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y))); // vertical input from right joystick
    // use fwd and turn to calculate voltage to send to motors
    left *= fwd - turn;
    right *= fwd + turn;

    setV(); // set voltage to motors
};

// turn off motors
void traditional_drive::stop()
{
    // brake for both groups
    left_side->brake();
    right_side->brake();
};

// set velocity to motors
void traditional_drive::setV()
{
    left_side->move_velocity(left);
    right_side->move_velocity(right);
    left = right = maxspeed; // reset rpm to be multiplied by scalar
};

// operator control arcade drive
void traditional_drive::arcade_drive()
{
    // get joystick values and apply square scaling
    fwd = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    // turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_X))) / 2.1; // horizontal input from right joystick
    turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_X)), 3) / 2;
    // turn = sin_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_X)), 2);
    // use fwd and turn to calculate voltage to send to motors
    left *= fwd + turn;
    right *= fwd - turn;

    setV(); // set velocity to motors
};

void traditional_drive::robot_centric_move(pair<double, double> movement_vector)
{
    // rads = deg * pi / 180
    movement_vector.second *= M_PI / 180;

    // x and y components of movement vector
    double x = cos(movement_vector.second) * movement_vector.first; 
    double y = sin(movement_vector.second) * movement_vector.first;

    // determine voltage to send to motors
    left *=(y - x);
    right *=(y + x);

    // send velocity to motors
    setV();
}

void traditional_drive::field_centric_move(pair<double, double> movement_vector)
{
    // get heading from inertial sensor and subtract from desired angle
    movement_vector.second = movement_vector.second + imu_->get_heading() - 360; 

    // call robot centric move with adjusted movement vector
    robot_centric_move(movement_vector);
}

// copy/pasted
void traditional_drive::app_move(std::pair<double, double> mag_angle_vector, double turn_rpm, double maxRPM, bool reversed) {
    // Prioritize turning by maintaining rotationalRPM difference (rotationalRPM * 2)
    double leftRPM;
    double rightRPM;
    double translationalRPM = mag_angle_vector.first;
    double rotationalRPM = mag_angle_vector.second;
    if (translationalRPM + std::abs(rotationalRPM) > maxRPM) {
        // Limit translational velocity when left or rightRPM would be > maxRPM by
        // maintaining the difference between left and rightRPM by replacing 
        // translationalRPM with maxRPM - abs(rotationalRPM)

        // this also means that if the robot is not within 
        // (maxRPM / p) degrees of its desired angle the robot
        // will only rotate until it gets within that range.
        leftRPM = maxRPM - std::abs(rotationalRPM) + rotationalRPM;
        rightRPM = maxRPM - std::abs(rotationalRPM) - rotationalRPM;
    } else {
        leftRPM = translationalRPM + rotationalRPM;
        rightRPM = translationalRPM - rotationalRPM;
    }
    // if (printMessages) pros::lcd::set_text(7, std::to_string(leftRPM));
    if (reversed) {
        get_motor_group(0).move_velocity(-rightRPM);
        get_motor_group(1).move_velocity(-leftRPM);
    } else {
        get_motor_group(0).move_velocity(leftRPM);
        get_motor_group(1).move_velocity(rightRPM);
    }
}

void traditional_drive::turn_with_power(double power)
{
    // multiply voltage by power factor
    // left*=power;
    // right=-(std::abs(right)*power);

    left_side->move(power);
    right_side->move(-power);

    // send voltage to motors
    // setV();
}

/**
 * Moves the robot.
 *
 * @param power The power to moves with normalized to [-1, 1] where +/- 1 is the maximum movement speed.
 * Positive for forwards, negative for backwards.
 *
 * @return Moves the robot with a rotational speed relative to power
 */
void traditional_drive::move_with_power(double power)
{
    // multiply voltage by power factor
    // left*=power;
    // right=-(std::abs(right)*power);

    left_side->move(power);
    right_side->move(power);

    // send voltage to motors
    // setV();
}

void traditional_drive::tank_with_power(double latPower, double turnPower)
{
    left_side->move(latPower+turnPower);
    right_side->move(latPower-turnPower);
}

void traditional_drive::split_tank_with_power(double leftPow, double rightPow)
{
    left_side->move(leftPow);
    right_side->move(rightPow);
}

Motor_Group& traditional_drive::get_motor_group(bool side)
{
    if (side == 0)// left
        return *left_side;
    else // right
        return *right_side;
}

Imu& traditional_drive::get_imu() 
{
    return *imu_;
}

Controller& traditional_drive::get_controller() 
{
    return *master;
}

double traditional_drive::getX() 
{
    if (odom_inst_ == nullptr) {
        pros::lcd::set_text(3, "trad_drive getX but odom is nullptr!!!");
        return -999;
    }

    return odom_inst_->getX();
}

double traditional_drive::getY() 
{
    if (odom_inst_ == nullptr) {
        pros::lcd::set_text(3, "trad_drive getY but odom is nullptr!!!");
        return -999;
    }

    return odom_inst_->getY();
}

Odom& traditional_drive::getOdom() {
    if (odom_inst_ == nullptr) {
        pros::lcd::set_text(6, "ERROR: getOdom but it's nullptr!!!");
        pros::delay(100);
        throw std::runtime_error("Called getOdom but odom_inst is null!");
    }

    return *odom_inst_;
}

void traditional_drive::brake() {
    left_side->set_brake_modes(BRAKETYPE_BRAKE);
    right_side->set_brake_modes(BRAKETYPE_BRAKE);
    left_side->brake();
    right_side->brake();
}