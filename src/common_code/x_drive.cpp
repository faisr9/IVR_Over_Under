#include "x_drive.h"

const double pi = 3.14159265358979323846;  //cant find pi constant in math

// add in a super for parent drive class later
x_drive::x_drive(Controller& master, Motor& front_left, Motor& front_right, Motor& back_left, Motor& back_right, Imu& imu) : 
    master_(master), front_left_(front_left), front_right_(front_right), back_left_(back_left), back_right_(back_right), DriveParent(imu, "x_drive"){}
 
void x_drive::robot_centric_move(std::pair<double, double> movement_vector, double turn_right_x){
    //angle uses radians since I use cos and sin
    // 0 is the right of the robot and pi is forward

    //splitting the movement_vector for easy use
    double magnitude = movement_vector.first;
    double angle = movement_vector.second;

    //idea is that magnitude controls speed in 
    double move_x = magnitude * cos(angle);
    double move_y = magnitude * sin(angle);

    double left_move = (move_y - move_x);
    double right_move = (move_y + move_x);
    
    front_left_.move(right_move + turn_right_x);
    front_right_.move(left_move - turn_right_x);
    back_left_.move(left_move + turn_right_x);
    back_right_.move(right_move - turn_right_x);

    // Total move amount (the square root of robot x^2 + robot x^y) is 2(root 2) times the initial magnitude
    // This means a magnitude of 1 and theta of pi (with 0 turn) would cause robot motors to all move by 1 and finding the total x and y components,
    // you get a y of 2 root 2 and x of 0 (since going forward) which leaves us a total of 2root2.
}

void x_drive::field_centric_move(std::pair<double, double> movement_vector, double turn_right_x){
    //flip the imu angle and convert it to radians
    double radians_from_imu = pi * (360.0 - imu_.get_heading()) / 180.0; 

    //adjusts angle based on imu
    double adjusted_angle = movement_vector.second - radians_from_imu;

    std::pair<double, double> new_movement_vector(movement_vector.first, adjusted_angle);
    this->robot_centric_move(new_movement_vector,turn_right_x);
}

void x_drive::robot_centric_move(std::pair<double, double> movement_vector){
    //Drive Parent uses degrees so I need to swap to radians
    std::pair<double, double> new_movement_vector(movement_vector.first, pi * movement_vector.second / 180.0);
    robot_centric_move(new_movement_vector,0);
}

void x_drive::field_centric_move(std::pair<double, double> movement_vector, double turn_right_x){
    //Drive Parent uses degrees so I need to swap to radians
    std::pair<double, double> new_movement_vector(movement_vector.first, pi * movement_vector.second / 180.0);
    field_centric_move(new_movement_vector,0);
}
void x_drive::turn_with_power(double power){
    std::pair<double, double> new_movement_vector(0.0, 0.0);
    robot_centric_move(new_movement_vector,power);
}