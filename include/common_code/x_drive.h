#pragma once

#include "api.h"
#include <cmath>
using namespace pros;
class x_drive{
    public:
        x_drive(Controller master, Motor front_left, Motor front_right, Motor back_left, Motor back_right, Imu imu);
        void robot_centric_move(std::pair<double, double> movement_vector);
        void field_centric_move(std::pair<double, double> movement_vector);
        
    private:
        Controller master_;
        Motor front_left_;
        Motor front_right_;
        Motor back_left_;
        Motor back_right_;
        IMU imu_;
};