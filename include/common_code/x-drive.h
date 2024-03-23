#pragma once
#include "drive_parent.h"
#include "api.h"
#include <cmath>

using namespace pros;
class x_drive : public DriveParent
{
public:
    x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Imu &imu);
    void turn_with_power(double power);                                 // turn in place (movement_vector = 0)
    void robot_centric_move(std::pair<double, double> movement_vector, double turn_right_x=0);
    void field_centric_move(std::pair<double, double> movement_vector, double turn_right_x=0);

private:
    Controller master_;
    Motor front_left_;
    Motor front_right_;
    Motor back_left_;
    Motor back_right_;
};