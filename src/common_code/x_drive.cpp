#include "x-drive.h"

// add in a super for parent drive class later
x_drive::x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Imu &imu) : master_(master), front_left_(front_left), front_right_(front_right), back_left_(back_left), back_right_(back_right), DriveParent(imu, "x_drive") {}

void x_drive::robot_centric_move(std::pair<double, double> movement_vector, double turn_right_x)
{
    // cos and sin (cmath) use radians
    // 0 is the right of the robot and π is forward

    // splitting the movement_vector
    double magnitude = movement_vector.first;
    double angle = movement_vector.second;

    // magnitude scales direction
    double move_x = magnitude * cos(angle);
    double move_y = magnitude * sin(angle);

    double left_move = (move_y - move_x);
    double right_move = (move_y + move_x);

    front_left_.move(right_move + turn_right_x);
    front_right_.move(left_move - turn_right_x);
    back_left_.move(left_move + turn_right_x);
    back_right_.move(right_move - turn_right_x);

    // Total move amount √(x²+y²) is 2√2 times the initial magnitude
    // This means a magnitude of 1 and theta of pi (with 0 turn) would cause robot motors to all move by 1 and finding the total x and y components,
    // you get a y = 2√2 and x = 0 (forward) which produces a total of 2√2.
}

void x_drive::field_centric_move(pair<double, double> movement_vector, double turn_right_x)
{
    // flip the imu angle and convert to radians
    double radians_from_imu = M_PI * (360.0 - imu_.get_heading()) / 180.0;

    // adjusts angle based on imu
    double adjusted_angle = movement_vector.second - radians_from_imu;

    pair<double, double> new_movement_vector(movement_vector.first, adjusted_angle);
    this->robot_centric_move(new_movement_vector, turn_right_x);
}

void x_drive::robot_centric_move(std::pair<double, double> movement_vector)
{
    // Drive Parent uses degrees -> swap to radians
    pair<double, double> new_movement_vector(movement_vector.first, M_PI * movement_vector.second / 180.0);
    robot_centric_move(new_movement_vector, 0);
}

void x_drive::field_centric_move(pair<double, double> movement_vector, double turn_right_x)
{
    // Drive Parent uses degrees -> swap to radians
    pair<double, double> new_movement_vector(movement_vector.first, M_PI * movement_vector.second / 180.0);
    field_centric_move(new_movement_vector, 0);
}
void x_drive::turn_with_power(double power)
{
    pair<double, double> new_movement_vector(0.0, 0.0);
    robot_centric_move(new_movement_vector, power);
}