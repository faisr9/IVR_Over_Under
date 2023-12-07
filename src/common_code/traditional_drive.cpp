//
// Description: implementations for traditional drive system
// Path: src/common_code/traditional_drive.cpp
// Header: include/common_code/traditional_drive.h
// Last Modified: 12/1/23 by Zach Martin
//

#include "common_code/traditional_drive.h"
#include <cmath> // for sin and cos functions and M_PI
//
// ************ overloaded constructors ************
//
// default to arcade drive and then run constructor with mode and controller
traditional_drive::traditional_drive(Imu&imu,Controller &mstr, Motor_Group &l, Motor_Group &r) 
    : DriveParent(imu, drive_mode[0])
{
    master = &mstr;
    this->imu=&imu;
    left_side = &l; 
    right_side = &r;
    toggle_drive_mode(0);

};
// default to arcade drive and then run constructor with mode without controller
traditional_drive::traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r)
    : DriveParent(imu, drive_mode[0])
{
    this->imu=&imu;
    left_side = &l;
    right_side = &r;
    toggle_drive_mode(0);

};
// initialize controller if applicable
traditional_drive::traditional_drive(Imu &imu, Controller &mstr, Motor_Group &l, Motor_Group &r, int mode)
    : DriveParent(imu, drive_mode[mode])
{
    master = &mstr;
    this->imu=&imu;
    left_side = &l; 
    right_side = &r;
    toggle_drive_mode(mode);
};
// initialize variables
traditional_drive::traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, int mode)
    : DriveParent(imu, drive_mode[mode])
{
    // set controller and motor groups
    this->imu=&imu;
    left_side = &l;
    right_side = &r;
    toggle_drive_mode(mode);
};

// ************ destructor ************
traditional_drive::~traditional_drive()
{
    // turn off motors
    stop();
};


// toggle drive mode (arcade, tank, hybrid)
void traditional_drive::toggle_drive_mode(int mode = 0)
{
    // 0 = arcade, 1 = tank, 2 = hybrid
    switch (mode)
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
    left *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    right *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y))); // vertical input from right joystick

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
    // set voltage to 0 for both groups
    left = 0;
    right = 0;
    setV();
};

// set voltage to motors
void traditional_drive::setV()
{
    left_side->move_voltage(left);
    right_side->move_voltage(right);
    left = right = scalingFactor; // reset voltage to be multiplied by scalar
};

// operator control arcade drive
void traditional_drive::arcade_drive()
{
    // get joystick values and apply square scaling
    fwd = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_X))); // horizontal input from right joystick
    // use fwd and turn to calculate voltage to send to motors
    left *= fwd + turn;
    right *= fwd - turn;

    setV(); // set voltage to motors
};


/**
 * @param mag_angle_vector An std::pair of doubles containing (first) the magnitude of the desired
 * movement vector and (second) the angle of the movement in [0, 360) in degrees.
 * To only turn the robot simply pass a magnitude of 0 and the desired angle.
 *
 * @return Moves the robot according to the given std::pair
 */
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

    // send voltage to motors
    setV();
}


/**
 * @param mag_angle_vector An std::pair of doubles containing (first) the magnitude of the desired
 * movement vector and (second) the angle of the movement in [0, 360) in degrees.
 * To only turn the robot simply pass a magnitude of 0 and the desired angle.
 *
 * @return Moves the robot according to the given std::pair
 */
void traditional_drive::field_centric_move(pair<double, double> movement_vector)
{
    // get heading from inertial sensor and subtract from desired angle
    movement_vector.second = movement_vector.second + imu->get_heading() - 360; 

    // call robot centric move with adjusted movement vector
    robot_centric_move(movement_vector);
}


/**
 * Turns the robot on a point.
 *
 * @param power The power to turn with normalized to [-1, 1] where +/- 1 is the maximum turning speed.
 * Positive for clockwise (increasing theta), negative for counterclockwise (decreasing theta).
 *
 * @return Turns the robot with a rotational speed relative to power
 */
void traditional_drive::turn_with_power(double power)
{
    // multiply voltage by power factor
    left*=power;
    right*=power;

    // send voltage to motors
    setV();
}
Motor_Group& traditional_drive::get_motor_group(bool side)
{
    if (side == 0)
        return *left_side;
    else
        return *right_side;
}