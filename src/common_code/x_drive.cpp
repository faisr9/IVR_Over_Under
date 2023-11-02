#include "x_drive.h"


// add in a super for parent drive class later
x_drive::x_drive(Controller master, Motor front_left, Motor front_right, Motor back_left, Motor back_right, Imu imu) : 
    master_(master), front_left_(front_left), front_right_(front_right), back_left_(back_left), back_right_(back_right), imu_(imu) {}
 
void x_drive::robot_centric_move(std::pair<double, double> movement_vector){
    double magnitude = movement_vector.first;
    double angle = movement_vector.second;

    //need to double check the sqrt 2 thing but this should work
    double move_x = magnitude * cos(angle) / sqrt(2.0);
    double move_y = magnitude * sin(angle) / sqrt(2.0);

    double kmultiplier = 5.5;
    double left_move = kmultiplier*(move_y - move_x);
    double right_move = kmultiplier*(move_y + move_x);
    front_left_.move(right_move);
    front_right_.move(left_move);
    back_left_.move(left_move);
    back_right_.move(right_move);

}

void x_drive::field_centric_move(std::pair<double, double> movement_vector){
    const double pi = 3.14159265358979323846; //cant find pi constant in math

    double radians_from_imu = pi * (360.0 - imu_.get_heading()) / 180.0; //left is 90
    double adjusted_angle = movement_vector.second - radians_from_imu;

    std::pair<double, double> new_movement_vector(movement_vector.first, adjusted_angle);
    this->robot_centric_move(new_movement_vector);
}