/*
 * Description: traditional drive system
 * Dependencies: drive_parent.h, odom.h
 * Path: include/common_code/traditional_drive.h
 * Implementation: src/common_code/traditional_drive.cpp
 * Last Modified: 04/14/24 by Zach Martin
 */

#pragma once
#include "common_code/drive_parent.h"
#include "common_code/odom.h"
#include "main.h"

// drive systems
class traditional_drive : public DriveParent
{
    private:
        double maxspeed = 200.0;    // max speed of motors based on gearing
        double left=maxspeed, right=maxspeed; // velocity to send to motors (scalar factor of maxspeed)
        double fwd, turn; //helper variables
        Controller *master_; // controller to get input from
        Imu *imu_; // inertial sensor to get angle from
        std::string drive_mode[3] = {"arcade", "tank", "hybrid"}; // drive mode string names
        int mode_; // current drive mode
        Motor_Group *left_side_, *right_side_; // motor groups to send voltage to
        Odom* odom_inst_ = nullptr; // instance of the odom class on the heap
        /*
         * called by constructors to initialize variables
         * @param imu inertial sensor to get angle from
         * @param l left motor group
         * @param r right motor group
         * @param mode drive mode (mode 0 = arcade, 1 = tank, 2 = hybrid)
         */
        void init(Imu &imu, Motor_Group &l, Motor_Group &r, int mode);
        // drive mode methods
        /*
         * arcade drive method
         */
        void arcade_drive();
        /*
         * tank drive method
         */
        void tank_drive();
        /*
         * hybrid drive method
         */
        void hybrid_drive();
        /*
         * stop motors using brake method
         */
        void stop();
        /*
         * set velocity to motors
         */
        void setV();

    public:
        /*
         * initialize controller if applicable
         * @param imu inertial sensor to get angle from
         * @param mstr controller to get input from
         * @param l left motor group
         * @param r right motor group
         * @param mode drive mode (mode 0 = arcade, 1 = tank, 2 = hybrid)
         */
        traditional_drive(Imu &imu, Controller &mstr, Motor_Group &l, Motor_Group &r, int mode); // initialize controller
        /*
         * most basic constructor that initializes all required variables
         * @param imu inertial sensor to get angle from
         * @param l left motor group
         * @param r right motor group
         * @param mode drive mode (mode 0 = arcade, 1 = tank, 2 = hybrid)
         */
        traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, int mode); // initialize variables
        /*
         * Constructor with odometry and no controller
         * @param imu inertial sensor to get angle from
         * @param l left motor group
         * @param r right motor group
         * @param odometry instance of the odom class
         */
        traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, Odom& odometry);
        /*
         * Constructor with odometry and controller
         * @param imu inertial sensor to get angle from
         * @param mstr controller to get input from
         * @param l left motor group
         * @param r right motor group
         * @param odometry instance of the odom class
         */
        traditional_drive(Imu&imu,Controller &mstr, Motor_Group &l, Motor_Group &r, Odom& odometry);
        /*
         * destructor
         */
        ~traditional_drive();
        /*
         * toggle drive mode (arcade, tank, hybrid)
         * performs one cycle of movement based on the current drive mode
         */
        void toggle_drive_mode();
        /*
         * move in robot centric coordinates
         * @param mag_angle_vector An std::pair of doubles containing (first) the magnitude of the desired
         * movement vector and (second) the angle of the movement in [0, 360) in degrees.
         * To only turn the robot simply pass a magnitude of 0 and the desired angle.
         */
        void robot_centric_move(std::pair<double, double> mag_angle_vector);
        /*
         * move in field centric coordinates
         * calculated offset based on imu angle and modifies
         * movement_vector, then calls robot_centric_move
         * with the modified movement_vector
         * 
         * @param mag_angle_vector An std::pair of doubles containing (first) the magnitude of the desired
         * movement vector and (second) the angle of the movement in [0, 360) in degrees.
         * To only turn the robot simply pass a magnitude of 0 and the desired angle.
         */
        void field_centric_move(std::pair<double, double> mag_angle_vector);
        /**
         * Turns the robot on a point.
         *
         * @param power The power to turn with normalized to [-1, 1] where +/- 1 is the maximum turning speed.
         * Positive for clockwise (increasing theta), negative for counterclockwise (decreasing theta).
         * @return Turns the robot with a rotational speed relative to power
         */
        void turn_with_power(double power);
        
        void move_with_power(double power);
        void tank_with_power(double latPower, double turnPower);

        void app_move(std::pair<double, double> mag_angle_vector, double max_rpm, bool reversed);
        /**
         * Runs the drive system
         * mode 0 = arcade, 1 = tank, 2 = hybrid
         * @return void
         */
        void change_drive_mode(int mode);
        /*
         * getter function for motor groups
         * @param side true for left, false for right
         * @return motor group
         */
        Motor_Group &get_motor_group(bool side);
        /*
         * getter function for imu
         * @return imu
         */
        Imu& get_imu();
        /*
         * getter function for controller
         * @return controller
         */
        Controller& get_controller();
        /*
         * getter function for x-coordinate
         * @return x-ccordinate
         */
        double getX();
        /*
         * getter function for y-coordinate
         * @return y-coordinate
         */
        double getY();
        /*
         * getter function for odom
         * @return odom
         */
        Odom& getOdom();
};