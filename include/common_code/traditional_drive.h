//
// Description: traditional drive system
// Dependencies: main.h, subsystem_parent.h
// Path: include/common_code/traditional_drive.h
// Implementation: src/common_code/traditional_drive.cpp
// Last Modified: 11/29/23 by Zach Martin
//

#pragma once
#include "main.h"
#include "drive_parent.h"
using namespace std;
using namespace pros;

// drive systems
class traditional_drive : public DriveParent
{
    private:

        double left=12000, right=12000; // voltage to send to motors (scalar factor of 12000)
        double fwd, turn; //helper variables
        Controller *master; // controller to get input from
        Imu *imu; // inertial sensor to get angle from
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
        traditional_drive(Imu&imu,Controller *mstr, Motor_Group *l, Motor_Group *r); // converts to other constructor
        traditional_drive(Imu &imu,Controller *mstr, Motor_Group *l, Motor_Group *r, int mode); // initialize variables
        ~traditional_drive(); // destructor

        void toggle_drive_mode(int mode); // toggle drive mode (arcade, tank, hybrid)
        void robot_centric_move(pair<double, double> mag_angle_vector); 
        void field_centric_move(pair<double, double> mag_angle_vector);
        void turn_with_power(double power);
};