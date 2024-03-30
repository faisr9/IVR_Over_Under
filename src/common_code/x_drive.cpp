#include "x-drive.h"

// add in a super for parent drive class later
x_drive::x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Imu &imu) : master_(master), front_left_(front_left), front_right_(front_right), back_left_(back_left), back_right_(back_right), DriveParent(imu, "x_drive") {}

void x_drive::robot_centric_move(pair<double, double> movement_vector, double turn)
{
    auto speed = 0.0;
    if (movement_vector.first) speed=127.0*movement_vector.first; // normalized speed of movement times max speed
    else if (turn) speed=127.0; // if only turning, set speed to max
    auto dir = movement_vector.second; // direction in radians

    dir -= M_PI / 4; // adjust direction by 45˚ to get the diagonal components of movement
    auto move_1 = -1*cos(dir); // first diagonal component of movement
    auto move_2 = sin(dir); // second diagonal component of movement

    auto scaling = speed/max(abs(move_1), abs(move_2)); // speed divided by max of x and y
    auto turn_factor = 1 - abs(turn); // turn factor

    front_left_.move((move_1*scaling*turn_factor - speed*turn)); // fl and br use the first diagonal component
    front_right_.move((move_2*scaling*turn_factor - speed*turn)); // front motors subtract turn
    back_left_.move((move_2*scaling*turn_factor + speed*turn)); // bl and fr use the second diagonal component
    back_right_.move(-(move_1*scaling*turn_factor + speed*turn)); // back motors add turn
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
    pair<double, double> target(0.0, 0.0);
    auto x=master_.get_analog(E_CONTROLLER_ANALOG_LEFT_X); // target x
    auto y=master_.get_analog(E_CONTROLLER_ANALOG_LEFT_Y); // target y

    if (x || y ) // if there is any movement
    {
        target.first = min(1.0,sqrt(pow(x,2) + pow(y,2)) / 127.0);///sqrt(2)); // normalized speed to move
        target.second = atan2(y, x); // direction to move in radians
    }
    
    auto turn = master_.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)/127.0; // normalized turn amount
    field_centric_move(target, turn); // move the robot
}