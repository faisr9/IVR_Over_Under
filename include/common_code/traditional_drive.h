//
// Description: traditional drive system
// Dependencies: main.h, drive_parent.h, cmath
// Path: include/common_code/traditional_drive.h
// Implementation: src/common_code/traditional_drive.cpp
// Last Modified: 12/1/23 by Zach Martin
//

#pragma once
#include "main.h"
#include "drive_parent.h" // base class
#include "common_code/odom.h"
using namespace std; // for string
using namespace pros; // for Controller and Imu

// drive systems
class traditional_drive : public DriveParent
{
    private:
        double scalingFactor=12000; // scalar factor for voltage
        double left=scalingFactor, right=scalingFactor; // voltage to send to motors (scalar factor of 12000)
        double fwd, turn; //helper variables
        Controller *master; // controller to get input from
        Imu *imu; // inertial sensor to get angle from
        string drive_mode[3] = {"arcade", "tank", "hybrid"}; // drive mode names
        Motor_Group *left_side, *right_side; // motor groups to send voltage to
        Odom *odom_inst = nullptr; // instance of the odom class on the heap
        void init(Imu &imu, Motor_Group &l, Motor_Group &r, int mode);
        // drive mode methods
        void arcade_drive();
        void tank_drive();
        void hybrid_drive();

        void stop(); // turn off motors

        void setV(); // set voltage to motors

    public:
        // constructors
        // traditional_drive(): SubsystemParent(drive_mode[0]){}; // default constructor
        // overloaded constructors
        traditional_drive(Imu&imu,Controller &mstr, Motor_Group &l, Motor_Group &r); // converts to other constructor
        traditional_drive(Imu &imu,Controller &mstr, Motor_Group &l, Motor_Group &r, int mode); // initialize controller
        // no controller
        traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r); // converts to other constructor
        traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, int mode); // initialize variables

        traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, Odom* odometry); // with odom no controller
        traditional_drive(Imu&imu,Controller &mstr, Motor_Group &l, Motor_Group &r, Odom* odometry); // everything

        ~traditional_drive(); // destructor

        void toggle_drive_mode(int mode); // toggle drive mode (arcade, tank, hybrid)
        void robot_centric_move(pair<double, double> mag_angle_vector); 
        void field_centric_move(pair<double, double> mag_angle_vector);
        void turn_with_power(double power);
        Motor_Group &get_motor_group(bool side);
        Imu& get_imu();
        Controller& get_controller();
        double getX();
        double getY();
};