#include "x-drive.h"

class asterisk_drive : protected x_drive
{
public:
    void robot_centric_move(pair<double, double> movement_vector, double turn);
    /*
     * asterisk drive constructor
     * (holonomic x-drive modified to include foward wheels for pushing force)
     * @param master controller to get input from
     * @param front_left motor of front left wheel
     * @param front_right motor of front right wheel
     * @param back_left motor of back left wheel
     * @param back_right motor of back right wheel
     * @param straight_left_A first motor for left straight wheel (exclusive to asterisk drive)
     * @param straight_right_A first motor for right straight wheel (exclusive to asterisk drive)
     * @param straight_left_B second motor for left straight wheel (exclusive to asterisk drive)
     * @param straight_right_B second motor for right straight wheel (exclusive to asterisk drive)
     * @param imu inertial sensor to get angle from
     */
    asterisk_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, MotorGroup &straight_right_, MotorGroup &straight_left_, Imu &imu);

private:
    // forward wheels for asterisk drive
    Motor straight_left_A_;  // first left straight motor
    Motor straight_right_A_; // first right straight motor
    Motor straight_left_B_;  // second left straight motor
    Motor straight_right_B_; // second right straight motor
    double maxspeed = 200.0; // max speed of motors
};