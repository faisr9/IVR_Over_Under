#include "x-drive.h"

// constructor with no foward wheels
x_drive::x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Imu &imu) : master_(master), front_left_(front_left), front_right_(front_right), back_left_(back_left), back_right_(back_right), DriveParent(imu, "x_drive") {}

// astrisk drive constructor (x-drive modified to include foward wheels for pushing force)
x_drive::x_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Motor &straight_left_A, Motor &straight_right_A, Motor &straight_left_B, Motor &straight_right_B, Imu &imu) : master_(master), front_left_(front_left), front_right_(front_right), back_left_(back_left), back_right_(back_right), straight_left_A_(straight_left_A), straight_right_A_(straight_right_A), straight_left_B_(straight_left_B), straight_right_B_(straight_right_B), DriveParent(imu, "x_drive") {}

void x_drive::robot_centric_move(pair<double, double> movement_vector, double turn)
{
    auto k = front_left_.get_gearing();
    auto maxspeed = 0.0;

    if (k == 0)
        maxspeed = 100.0;
    else if (k == 1)
        maxspeed = 200.0;
    else if (k == 2)
        maxspeed = 600.0;

    auto speed = 0.0;
    if (movement_vector.first)
        speed = 127.0 * movement_vector.first; // normalized speed of movement times max speed
    else if (turn)
        speed = 127.0;                 // if only turning, set speed to max
    auto dir = movement_vector.second; // direction in radians

    dir -= M_PI / 4;             // adjust direction by 45˚ to get the diagonal components of movement
    auto move_1 = -1 * cos(dir); // first diagonal component of movement
    auto move_2 = sin(dir);      // second diagonal component of movement

    auto scaling = speed / max(abs(move_1), abs(move_2)); // speed divided by max of x and y
    auto turn_factor = 1 - abs(turn);                     // turn factor

    auto fl_move = (move_1 * scaling * turn_factor - speed * turn); // fl and br use the first diagonal component
    auto fr_move = (move_2 * scaling * turn_factor - speed * turn); // front motors subtract turn
    auto bl_move = (move_2 * scaling * turn_factor + speed * turn); // bl and fr use the second diagonal component
    // direction probably needs to be flipped depending on the orientation of the motors
    auto br_move = -(move_1 * scaling * turn_factor + speed * turn); // back motors add turn

    // move the four primary motors        
    front_left_.move(fl_move);
    front_right_.move(fr_move);
    back_left_.move(bl_move);
    back_right_.move(br_move);

    // if there are forward wheels, then perform asterisk drive
    if (straight_left_A_.get_port() != -1 && straight_right_A_.get_port() != -1 && straight_left_B_.get_port() != -1 && straight_right_B_.get_port() != -1)
    {
        // left separate from calculations to make
        // debugging easier if the wheels are misaligned
        auto theta = M_PI / 4; // assuming perfect 45˚ angle

        // will need to make negative if the wheels are facing the opposite way
        auto sl_move = fl_move * cos(theta); // should be equivalent to dividing by √2
        auto sr_move = fr_move * cos(theta); // should be equivalent to dividing by √2

        // move forward/straight wheels
        straight_left_A_.move(sl_move);
        straight_right_A_.move(sr_move);
        straight_left_B_.move(sl_move);
        straight_right_B_.move(sr_move);
    }
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
        target.first = min(1.0, sqrt(pow(x, 2) + pow(y, 2)) / 127.0); /// sqrt(2)); // normalized speed to move
        target.second = atan2(y, x);                                  // direction to move in radians
    }

    auto turn = master_.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) / 127.0; // normalized turn amount
    field_centric_move(target, turn);                                    // move the robot
}