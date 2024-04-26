/*
 * Description: x-drive system
 * Dependencies: drive_parent.h, cmath
 * Path: include/common_code/x-drive.h
 * Implementation: src/common_code/x_drive.cpp
 * Last Modified: 04/14/24 by Zach Martin
 */
#pragma once
#include "drive_parent.h"
#include "main.h"
#include <cmath> // for angle calculations

using namespace pros;
using namespace std;

class x_drive : public DriveParent
{
public:
    /*
     * run method to be called in opcontrol
     * reads controller input and moves the robot
     * based on the input
     * @return void
     */
    void run();
    /*
     * constructor with no foward wheels (regular holonomic x-drive)
     * @param master controller to get input from
     * @param front_left front left motor
     * @param front_right front right motor
     * @param back_left back left motor
     * @param back_right back right motor
     * @param imu inertial sensor to get angle from
     */
    x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Imu &imu);
    /*
     * destructor for x-drive
     * stops motors and eliminates memory leaks
     */
    ~x_drive();
    /*
     * turn in place (movement_vector = 0)
     * calls robot_centric_move with movement_vector = {0.0,0.0}
     * @param power power to turn with
     * @return void
     */
    void turn_with_power(double power);
    /*
     * move in robot centric coordinates
     * @param movement_vector pair of magnitude and angle
     * @param turn_right_x turn right x
     * @return void
     */
    virtual void robot_centric_move(pair<double, double> movement_vector, double turn_right_x, double max_rpm = -1);
    /*
     * move in field centric coordinates
     * calculated offset based on imu angle and modifies
     * movement_vector, then calls robot_centric_move
     * with the modified movement_vector
     * @param movement_vector pair of magnitude and angle
     * @param turn_right_x turn right x
     */
    void field_centric_move(pair<double, double> movement_vector, double turn_right_x, double max_rpm = -1);
    /*
     * move in robot centric coordinates
     * calls robot_centric_move with turn = 0
     * @param movement_vector pair of magnitude and direction of movement
     * @return void
     */
    void robot_centric_move(pair<double, double> movement_vector) { robot_centric_move(movement_vector, 0); }
    /*
     * move in field centric coordinates
     * calls field_centric_move with turn = 0
     * @param movement_vector pair of magnitude and direction of movement
     * @return void
     */
    void field_centric_move(pair<double, double> movement_vector) { field_centric_move(movement_vector, 0); }
    /*
     * stop the robot
     * brake all motors
     */
    virtual void stop();
    virtual void app_move(std::pair<double, double> mag_angle_vector, double turn_rpm, double max_rpm, bool reversed);
    double get_max_rpm() const;
    double get_rpm_per_meter() const;

protected:
    Controller *master_; // controller to get input from

    // motors
    Motor *front_left_;  // front left motor
    Motor *front_right_; // front right motor
    Motor *back_left_;   // back left motor
    Motor *back_right_;  // back right motor
    double maxspeed = 200.0; // max speed of motors
    double rpm_per_meter = 400.0; // for pure pursuit
};