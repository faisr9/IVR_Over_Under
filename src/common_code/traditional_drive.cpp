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
    master_ = &mstr;
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
    master_ = &mstr;
    odom_inst_ = &odometry; 
    init(imu, l, r, 0);
}

// initialize variables
void traditional_drive::init(Imu &imu, Motor_Group &l, Motor_Group &r, int mode) 
{
    // set controller and motor groups
    imu_=&imu;
    left_side_ = &l;
    right_side_ = &r;
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
    master_ = nullptr;
    left_side_ = nullptr;
    right_side_ = nullptr;
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
    left *= square_scale(normalize_joystick(master_->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    right *= square_scale(normalize_joystick(master_->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y))); // vertical input from right joystick

    setV(); // set voltage to motors
};

// operator control hybrid drive
void traditional_drive::hybrid_drive()
{
    // get joystick values and apply square scaling
    fwd = square_scale(normalize_joystick(master_->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    turn = square_scale(normalize_joystick(master_->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y))); // vertical input from right joystick
    // use fwd and turn to calculate voltage to send to motors
    left *= fwd - turn;
    right *= fwd + turn;

    setV(); // set voltage to motors
};

// turn off motors
void traditional_drive::stop()
{
    // brake for both groups
    left_side_->brake();
    right_side_->brake();
};

// set velocity to motors
void traditional_drive::setV()
{
    left_side_->move_velocity(left);
    right_side_->move_velocity(right);
    left = right = maxspeed; // reset rpm to be multiplied by scalar
};

// operator control arcade drive
void traditional_drive::arcade_drive()
{
    // get joystick values and apply square scaling
    fwd = square_scale(normalize_joystick(master_->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    turn = square_scale(normalize_joystick(master_->get_analog(E_CONTROLLER_ANALOG_RIGHT_X))) / 1.5; // horizontal input from right joystick
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

void traditional_drive::turn_with_power(double power)
{
    // set velocity to motors
    left_side_->move(power);
    right_side_->move(-power);
}

Motor_Group& traditional_drive::get_motor_group(bool side)
{
    if (side == 0)// left
        return *left_side_;
    else // right
        return *right_side_;
}

Imu& traditional_drive::get_imu() 
{
    return *imu_;
}

Controller& traditional_drive::get_controller() 
{
    return *master_;
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
