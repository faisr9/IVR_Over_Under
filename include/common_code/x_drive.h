#pragma once
#include "drive_parent.h"
#include "api.h"
#include <cmath>

using namespace pros;
class x_drive : public DriveParent{
    public:
        x_drive(Controller& master, Motor& front_left, Motor& front_right, Motor& back_left, Motor& back_right, Imu& imu);
        void robot_centric_move(std::pair<double, double> movement_vector);//These 2 don't have turning to satisfy DriveParent
        void field_centric_move(std::pair<double, double> movement_vector);//I just pass into other funcs with turning = 0;
        void turn_with_power(double power); //This is just turn in place so movement vector = 0
        void robot_centric_move(std::pair<double, double> movement_vector, double turn_right_x);
        void field_centric_move(std::pair<double, double> movement_vector, double turn_right_x);
        
    private:
        Controller& master_;
        Motor& front_left_;
        Motor& front_right_;
        Motor& back_left_;
        Motor& back_right_;
};