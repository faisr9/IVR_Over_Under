//
// Description: traditional drive system
// Dependencies: main.h, subsystem_parent.h
// Path: include/common_code/traditional_drive.h
// Implementation: src/common_code/traditional_drive.cpp
// Last Modified: 11/9/23 by Zach Martin
//

#pragma once
#include "main.h"
#include "subsystem_parent.h"
using namespace std;
using namespace pros;

// normalize joystick values to -1 to 1
static double normalize_joystick(double input)
{
    return input / 127.0;
};
// apply smooth sin scaling
static double sin_scale(double input, double sin_scale_factor)
{
    return copysign(pow(sin((M_PI / 2) * fabs(input)), sin_scale_factor), input);
};
// apply square scaling
static double square_scale(double input)
{
    return copysign(pow(input, 2), input);
};

// drive systems
class traditional_drive : public SubsystemParent
{
    private:

        double left=12000, right=12000; // voltage to send to motors (scalar factor of 12000)
        double fwd, turn; //helper variables
        Controller *master; // controller to get input from
        string drive_mode[3] = {"arcade", "tank", "hybrid"}; // drive mode names
        Motor_Group *left_side, *right_side; // motor groups to send voltage to
        // drive mode methods
        void arcade_drive();
        void tank_drive();
        void hybrid_drive();

        void stop(); // turn off motors

        void setV(); // set voltage to motors

    public:
        // constructors
        //traditional_drive(): SubsystemParent(drive_mode[0]){}; // default constructor
        // overloaded constructors
        traditional_drive(Controller *mstr, Motor_Group *l, Motor_Group *r); // converts to other constructor
        traditional_drive(Controller *mstr, Motor_Group *l, Motor_Group *r, int mode); // initialize variables
        ~traditional_drive(); // destructor

        void toggle_drive_mode(int mode); // toggle drive mode (arcade, tank, hybrid)
};