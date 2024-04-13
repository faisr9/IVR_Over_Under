#include "x-drive.h"
#include <iostream>

// constructor with no foward wheels
x_drive::x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Imu &imu) : master_(master), front_left_(front_left), front_right_(front_right), back_left_(back_left), back_right_(back_right), DriveParent(imu, "x_drive") {}

void x_drive::robot_centric_move(pair<double, double> movement_vector, double turn)
{
    auto k = front_left_.get_gearing(); // assume all motors have the same gearing
    auto maxspeed = 0.0; // max speed of motors based on gearing

    // set max speed based on gear
    if (k == 0) // 36:1
        maxspeed = 100.0; // max rpm
    else if (k == 1) // 18:1
        maxspeed = 200.0; // max rpm
    else if (k == 2) // 6:1
        maxspeed = 600.0; // max rpm
    else
        maxspeed = 200.0;

    auto speed = 0.0;
    auto dir = movement_vector.second; // direction in radians

    auto move_1 = 0.0; // first diagonal component of movement
    auto move_2 = 0.0; // second diagonal component of movement
    auto scaling = 0.0; // scale factor for movement
    if (movement_vector.first > 0.2)
    {
        speed = maxspeed * movement_vector.first; // normalized speed of movement times max speed
        dir -= M_PI / 4;                                 // adjust direction by 45˚ to get the diagonal components of movement
        move_1 = -1 * cos(dir);                          // opposite of cosine of direction
        move_2 = sin(dir);                               // sine of direction
        scaling = speed / max(abs(move_1), abs(move_2)); // speed divided by max of x and y
    }
    
    auto move_1_scaled = move_1 * scaling; // move speed
    auto move_2_scaled = move_2 * scaling; // move speed
    auto turn_scaled = maxspeed * turn; // turn speed
    auto priority = 0.5; // priority of movement over turning

    // if the sum of the speeds is greater than the max speed, scale them down
    if (max(abs(move_1_scaled),abs(move_2_scaled)) + abs(turn_scaled) > maxspeed) 
    {
        move_1_scaled = move_1_scaled / (max(abs(move_1_scaled),abs(move_2_scaled)) + abs(turn_scaled)) * maxspeed;
        move_2_scaled = move_2_scaled / (max(abs(move_1_scaled), abs(move_2_scaled)) + abs(turn_scaled)) * maxspeed;
        turn_scaled = turn_scaled / (max(abs(move_1_scaled), abs(move_2_scaled)) + abs(turn_scaled)) * maxspeed;
    }
    auto fl_move = move_1_scaled - turn_scaled; // fl and br use the first diagonal component
    auto fr_move = move_2_scaled - turn_scaled; // front motors subtract turn
    auto bl_move = move_2_scaled + turn_scaled; // bl and fr use the second diagonal component
    auto br_move = move_1_scaled + turn_scaled; // back motors add turn

    // move the four primary motors
    front_left_.move_velocity(fl_move);
    front_right_.move_velocity(fr_move);
    back_left_.move_velocity(bl_move);
    back_right_.move_velocity(br_move);
}

void x_drive::field_centric_move(pair<double, double> movement_vector, double turn_right_x)
{
    // flip the imu angle and convert to radians
    double radians_from_imu = M_PI * (360.0 - imu_.get_heading()) / 180.0;

    // adjusts angle based on imu
    double adjusted_angle = movement_vector.second - radians_from_imu;

    pair<double, double> new_movement_vector(movement_vector.first, adjusted_angle);
    robot_centric_move(new_movement_vector, turn_right_x);
}

void x_drive::turn_with_power(double power)
{
    pair<double, double> new_movement_vector(0.0, 0.0); // no movement
    robot_centric_move(new_movement_vector, power);     // turn in place
}

void x_drive::run()
{
    pair<double, double> target(0.0, 0.0);                   // initialize target (magnitude, direction)
    auto x = master_.get_analog(E_CONTROLLER_ANALOG_LEFT_X); // target x-coordinate
    auto y = master_.get_analog(E_CONTROLLER_ANALOG_LEFT_Y); // target y-coordinate

    if (x || y) // if there is any movement
    {
        target.first = min(1.0, sqrt(pow(x, 2) + pow(y, 2)) / 127.0); // sqrt(2)); // normalized speed to move
        target.second = atan2(y, x);                                  // direction to move in radians
    }

    auto turn = master_.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) / 127.0; // normalized turn amount
    field_centric_move(target, turn);                                    // move the robot
}