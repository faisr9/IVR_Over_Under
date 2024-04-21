//
// Description: traditional drive system
// Dependencies: drive_parent.h
// Path: include/common_code/traditional_drive.h
// Implementation: src/common_code/traditional_drive.cpp
// Last Modified: 12/1/23 by Zach Martin
//

#pragma once
#include "common_code/drive_parent.h"
#include "common_code/odom.h"
#include "main.h"

// drive systems
class traditional_drive : public DriveParent
{
    private:
        double scalingFactor=12000; // scalar factor for voltage
        double left=scalingFactor, right=scalingFactor; // voltage to send to motors (scalar factor of 12000)
        double fwd, turn; //helper variables
        Controller *master; // controller to get input from
        Imu *imu; // inertial sensor to get angle from
        std::string drive_mode[3] = {"arcade", "tank", "hybrid"}; // drive mode names
        int mode;
        Motor_Group *left_side, *right_side; // motor groups to send voltage to
        Odom* odom_inst = nullptr; // instance of the odom class on the heap
        void init(Imu &imu, Motor_Group &l, Motor_Group &r, int mode);
        // drive mode methods
        void arcade_drive();
        void tank_drive();
        void hybrid_drive();

        void stop(); // turn off motors

        void setV(); // set voltage to motors

    public:
        // overloaded constructors
        traditional_drive(Imu &imu, Controller &mstr, Motor_Group &l, Motor_Group &r, int mode); // initialize controller
        traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, int mode); // initialize variables
        traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, Odom& odometry); // with odom no controller
        traditional_drive(Imu&imu,Controller &mstr, Motor_Group &l, Motor_Group &r, Odom& odometry); // everything

        ~traditional_drive(); // destructor

        void toggle_drive_mode(); //does one cycle of movement
        void robot_centric_move(std::pair<double, double> mag_angle_vector); 
        void field_centric_move(std::pair<double, double> mag_angle_vector);
        void turn_with_power(double power);
        void move_with_power(double power);
        void tank_with_power(double latPower, double turnPower);
        void change_drive_mode(int mode);
        Motor_Group &get_motor_group(bool side);
        Imu& get_imu();
        Controller& get_controller();
        double getX();
        double getY();
        Odom& getOdom();
        void brake();
};