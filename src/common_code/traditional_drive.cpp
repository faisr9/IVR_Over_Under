//
// Description: implementations for traditional drive system
// Path: src/common_code/traditional_drive.cpp
// Header: include/common_code/traditional_drive.h
// Last Modified: 11/9/23 by Zach Martin
//

#include "traditional_drive.h"

// default to arcade drive and then run constructor with mode
traditional_drive::traditional_drive(Controller *mstr, Motor_Group *l, Motor_Group *r) : traditional_drive(mstr, l, r, 0){};

// overloaded constructor
traditional_drive::traditional_drive(Controller *mstr, Motor_Group *l, Motor_Group *r, int mode) : SubsystemParent(drive_mode[mode])
{
    // set controller and motor groups
    master = mstr;
    left_side = l;
    right_side = r;
    toggle_drive_mode(mode);
};

// destructor
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
    left = right = 12000; // reset voltage to be multiplied by scalar
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