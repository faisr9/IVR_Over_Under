#include "x_drive.h"


// add in a super for parent drive class later
x_drive::x_drive(Controller master, Motor front_left, Motor front_right, Motor back_left, Motor back_right, Imu imu) : 
    master_(master), front_left_(front_left), front_right_(front_right), back_left_(back_left), back_right_(back_right), imu_(imu) {}
 
void x_drive::robot_centric_move(std::pair<double, double> movement_vector, double turn_right_x){
    double magnitude = movement_vector.first;
    double angle = movement_vector.second;
    pros::lcd::set_text(2, std::to_string(magnitude) + "  " + std::to_string(angle));
    double move_x;
    double move_y;
    if (angle == 0){
        move_x = 0;
        move_y = 0;
    }
    else{
        //need to double check the sqrt 2 thing but this should work
        move_x = magnitude * cos(angle);
        move_y = magnitude * sin(angle);
    }

    double kmultiplier = .9  / sqrt(2.0);
    double left_move = kmultiplier*(move_y - move_x);
    double right_move = kmultiplier*(move_y + move_x);
    
    front_left_.move(right_move + turn_right_x);
    front_right_.move(left_move - turn_right_x);
    back_left_.move(left_move + turn_right_x);
    back_right_.move(right_move - turn_right_x);

}

void x_drive::field_centric_move(std::pair<double, double> movement_vector, double turn_right_x){
    const double pi = 3.14159265358979323846; //cant find pi constant in math

    double radians_from_imu = pi * (360.0 - imu_.get_heading()) / 180.0; //left is 90
    double adjusted_angle = movement_vector.second - radians_from_imu;

    std::pair<double, double> new_movement_vector(movement_vector.first, adjusted_angle);
    this->robot_centric_move(new_movement_vector,turn_right_x);
}
