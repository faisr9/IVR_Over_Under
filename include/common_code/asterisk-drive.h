/*
 * Description: asterisk drive system
 * Dependencies: x-drive.h
 * Path: include/common_code/asterisk-drive.h
 * Implementation: src/common_code/asterisk-drive.cpp
 * Last Modified: 04/16/24 by Zach Martin
 */
/**************************************
 * CAUTION: THIS FILE HAS NOT BEEN TESTED
 * USE AT YOUR OWN RISK
 * MAY CONTAIN BUGS
 * MAY MODIFY YOUR SSN
 * MAY DELTE YOUR FAMILY
 * MAY RESET THE SIMULATION
 **************************************/
#pragma once
#include "x-drive.h"

class asterisk_drive : public x_drive
{
public:
    /*
     * robot centric movement for asterisk drive
     * same as x-drive but with forward wheels
     * @param movement_vector pair of magnitude and angle
     * @param turn turn right x
     */
    void robot_centric_move(pair<double, double> movement_vector, double turn_right_x, double max_rpm = -1);
    /*
     * asterisk drive constructor
     * (holonomic x-drive modified to include foward wheels for pushing force)
     * @param master controller to get input from
     * @param front_left motor of front left wheel
     * @param front_right motor of front right wheel
     * @param back_left motor of back left wheel
     * @param back_right motor of back right wheel
     * @param straight_left_ motor group (2) for left straight wheel (exclusive to asterisk drive)
     * @param straight_right_ motor group (2) for right straight wheel (exclusive to asterisk drive)
     * @param imu inertial sensor to get angle from
     */
    asterisk_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Motor_Group &straight_right_, Motor_Group &straight_left_, Imu &imu);

    //asterisk_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Motor& right_middle_1,Motor& right_middle_2, Motor& left_middle_1, Motor& left_middle_2, Imu &imu);
    /*
     * destructor for asterisk drive
     * stops motor and eliminates memory leaks
     */
    ~asterisk_drive();
    /*
     * stop motors using brake method
     */
    void stop();
    void app_move(std::pair<double, double> mag_angle_vector, double turn_rpm, double max_rpm, bool reversed);

private:
    // forward wheels for asterisk drive
    Motor_Group &straight_left_;  // group of left straight motors
    Motor_Group &straight_right_; // group of right straight motors
    double maxspeed = 200.0; // max speed of motors based on gearing
};