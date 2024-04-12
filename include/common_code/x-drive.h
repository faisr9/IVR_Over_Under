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
     * asterisk drive constructor
     * (holonomic x-drive modified to include foward wheels for pushing force)
     * @param master controller to get input from
     * @param front_left motor of front left wheel
     * @param front_right motor of front right wheel
     * @param back_left motor of back left wheel
     * @param back_right motor of back right wheel
     * @param straight_left_A first motor for left straight wheel (exclusive to asterisk drive)
     * @param straight_right_A first motor for right straight wheel (exclusive to asterisk drive)
     * @param straight_left_B second motor for left straight wheel (exclusive to asterisk drive)
     * @param straight_right_B second motor for right straight wheel (exclusive to asterisk drive)
     * @param imu inertial sensor to get angle from
     */
    x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Motor &straight_left_A_, Motor &straight_right_A, Motor &straight_left_B, Motor &straight_right_B, Imu &imu);
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
    void robot_centric_move(pair<double, double> movement_vector, double turn_right_x);
    /*
     * move in field centric coordinates
     * calculated offset based on imu angle and modifies
     * movement_vector, then calls robot_centric_move
     * with the modified movement_vector
     * @param movement_vector pair of magnitude and angle
     * @param turn_right_x turn right x
     */
    void field_centric_move(pair<double, double> movement_vector, double turn_right_x);
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

private:
    Controller master_; // controller to get input from

    // primary motors
    Motor front_left_;  // front left motor
    Motor front_right_; // front right motor
    Motor back_left_;   // back left motor
    Motor back_right_;  // back right motor

    // forward wheels for asterisk drive
    Motor straight_left_A_;  // first left straight motor
    Motor straight_right_A_; // first right straight motor
    Motor straight_left_B_;  // second left straight motor
    Motor straight_right_B_; // second right straight motor
};