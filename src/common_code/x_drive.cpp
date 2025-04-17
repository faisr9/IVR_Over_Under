/*
 * Description: implementations for x-drive system
 * Path: src/common_code/x_drive.cpp
 * Header: include/common_code/x_drive.h
 * Last Modified: 04/16/24 by Zach Martin
 */

#include "x-drive.h"
#include <iostream>

// constructor with no foward wheels
x_drive::x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Imu &imu) : master_(&master), front_left_(&front_left), front_right_(&front_right), back_left_(&back_left), back_right_(&back_right), DriveParent(imu, "x_drive") 
{
    auto gearing = front_left_->get_gearing(); // assume all motors have the same gearing

    // set max speed based on gear
    if (gearing == 0)      // 36:1
        maxspeed = 100.0;  // max rpm
    else if (gearing == 1) // 18:1
        maxspeed = 200.0;  // max rpm
    else if (gearing == 2) // 6:1
        maxspeed = 600.0;  // max rpm
    else
        maxspeed = 200.0; // default max rpm
}

x_drive::~x_drive()
{
    stop();
    front_left_ = nullptr;
    front_right_ = nullptr;
    back_left_ = nullptr;
    back_right_ = nullptr;
    master_ = nullptr;
}

void x_drive::robot_centric_move(pair<double, double> movement_vector, double turn, double max_rpm)
{
    if (max_rpm == -1) max_rpm = maxspeed;
    else max_rpm = std::min(max_rpm, maxspeed);
    auto speed = 0.0;
    auto dir = movement_vector.second; // direction in radians

    auto move_1 = 0.0; // first diagonal component of movement
    auto move_2 = 0.0; // second diagonal component of movement
    auto scaling = 0.0; // scale factor for movement
    if (movement_vector.first > 0.01) // consider joystick deadzone
    {
        speed = max_rpm * movement_vector.first; // normalized speed of movement times max speed
        dir -= M_PI / 4;                                 // adjust direction by 45˚ to get the diagonal components of movement
        move_1 = -1 * cos(dir);                          // opposite of cosine of direction
        move_2 = sin(dir);                               // sine of direction
        scaling = speed / max(abs(move_1), abs(move_2)); // speed divided by max of x and y
    }
    
    auto move_1_scaled = move_1 * scaling; // move speed
    auto move_2_scaled = move_2 * scaling; // move speed
    auto turn_scaled = max_rpm * turn; // turn speed
    auto priority = 1.0; // priority of movement over turning (1.0=equal priority)

    // if the sum of the speeds is greater than the max speed, scale them down
    if (max(abs(move_1_scaled),abs(move_2_scaled)) + abs(turn_scaled) > max_rpm) 
    {
        auto scale_factor = 1/(priority*max(abs(move_1_scaled),abs(move_2_scaled)) + abs(turn_scaled)) * max_rpm;
        move_1_scaled *= priority*scale_factor;
        move_2_scaled *= priority*scale_factor;
        turn_scaled *= scale_factor;
    }
    
    auto fl_move = move_1_scaled - turn_scaled; // fl and br use the first diagonal component
    auto fr_move = move_2_scaled - turn_scaled; // front motors subtract turn
    auto bl_move = move_2_scaled + turn_scaled; // bl and fr use the second diagonal component
    auto br_move = move_1_scaled + turn_scaled; // back motors add turn

    // move the four primary motors
    front_left_->move_velocity(fl_move);
    front_right_->move_velocity(fr_move);
    back_left_->move_velocity(bl_move);
    back_right_->move_velocity(br_move);
}

void x_drive::field_centric_move(pair<double, double> movement_vector, double turn_right_x, double max_rpm)
{
    // flip the imu angle and convert to radians
    double radians_from_imu = M_PI * (360.0 - imu_.get_heading()) / 180.0;

    // adjusts angle based on imu
    double adjusted_angle = movement_vector.second - radians_from_imu;

    pair<double, double> new_movement_vector(movement_vector.first, adjusted_angle);
    robot_centric_move(new_movement_vector, turn_right_x, max_rpm);
}

void x_drive::app_move(std::pair<double, double> mag_angle_vector, double turn_rpm, double max_rpm, bool reversed) {
    field_centric_move({mag_angle_vector.first / max_rpm, mag_angle_vector.second}, turn_rpm / max_rpm, max_rpm); 
}


void x_drive::stop()
{
    front_left_->brake();
    front_right_->brake();
    back_left_->brake();
    back_right_->brake();
}

double x_drive::get_max_rpm() const {
    return maxspeed;
}

double x_drive::get_rpm_per_meter() const {
    return rpm_per_meter;
}


void x_drive::turn_with_power(double power)
{
    pair<double, double> new_movement_vector(0.0, 0.0); // no movement
    robot_centric_move(new_movement_vector, power);     // turn in place
}

void x_drive::run()
{
    pair<double, double> target(0.0, 0.0);                   // initialize target (magnitude, direction)
    auto x = master_->get_analog(E_CONTROLLER_ANALOG_LEFT_X); // target x-coordinate
    auto y = master_->get_analog(E_CONTROLLER_ANALOG_LEFT_Y); // target y-coordinate

    if (x || y) // if there is any movement
    {
        target.first = min(1.0, sqrt(pow(x, 2) + pow(y, 2)) / 127.0); // sqrt(2)); // normalized speed to move
        target.second = atan2(y, x);                                  // direction to move in radians
    }

    double turn_driver_scale = 0.5;
    auto turn = master_->get_analog(E_CONTROLLER_ANALOG_RIGHT_X) / 127.0 * turn_driver_scale; // normalized turn amount scaled to driver preference
    field_centric_move(target, turn);                                    // move the robot
}