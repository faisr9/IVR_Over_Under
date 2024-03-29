#include "x-drive.h"

// add in a super for parent drive class later
x_drive::x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Imu &imu) : master_(master), front_left_(front_left), front_right_(front_right), back_left_(back_left), back_right_(back_right), DriveParent(imu, "x_drive") {}

void x_drive::robot_centric_move(pair<double, double> movement_vector, double turn_right_x)
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
    // This means a |movement|=1 and θ=π (turn=0) would cause robot motors to all move by 1 and finding the total x and y components,
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

void x_drive::turn_with_power(double power)
{
    pair<double, double> new_movement_vector(0.0, 0.0);
    robot_centric_move(new_movement_vector, power);
}

void x_drive::run()
{
    auto r=0.0,
        mag=0.0;
    if (pow(master_.get_analog(E_CONTROLLER_ANALOG_LEFT_X), 2) + pow(master_.get_analog(E_CONTROLLER_ANALOG_LEFT_Y), 2) > 0)
    {
        mag = sqrt(pow(master_.get_analog(E_CONTROLLER_ANALOG_LEFT_X), 2) + pow(master_.get_analog(E_CONTROLLER_ANALOG_LEFT_Y), 2));
        double param = master_.get_analog(E_CONTROLLER_ANALOG_LEFT_X) / mag;
        r = acos(param);
    }

    if (master_.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) <= 0)
    {
        r = 2 * M_PI - r;
    }

    pair<double, double> movement_vector(mag, r);
    double turn_right_x = (double)master_.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
    robot_centric_move(movement_vector, turn_right_x);
}