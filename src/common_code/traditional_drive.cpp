//
// Description: implementations for traditional drive system
// Path: src/common_code/traditional_drive.cpp
// Header: include/common_code/traditional_drive.h
// Last Modified: 12/1/23 by Zach Martin
//

#include "common_code/traditional_drive.h"
//
// ************ overloaded constructors ************
//

// most basic version that initializes all required variables
traditional_drive::traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, int m): DriveParent(imu, drive_mode[m]) {
    init(imu, l, r, mode);
};

// initialize controller if applicable
traditional_drive::traditional_drive(Imu &imu, Controller &mstr, Motor_Group &l, Motor_Group &r, int mode) : DriveParent(imu, drive_mode[mode])
{
    master = &mstr;
    init(imu, l, r, mode);
}

// with odometry
traditional_drive::traditional_drive(Imu &imu, Motor_Group &l, Motor_Group &r, Odom& odometry) : DriveParent(imu, drive_mode[0]) {
    odom_inst = &odometry;
    init(imu, l, r, 0);
} 
// with odom and controller
traditional_drive::traditional_drive(Imu&imu, Controller &mstr, Motor_Group &l, Motor_Group &r, Odom& odometry) : DriveParent(imu, drive_mode[0]) {
    master = &mstr;
    odom_inst = &odometry; 
    init(imu, l, r, 0);
}

void traditional_drive::init(Imu &imu, Motor_Group &l, Motor_Group &r, int mode) {
    // set controller and motor groups
    this->imu=&imu;
    left_side = &l;
    right_side = &r;
    this->mode = mode;
};

// ************ destructor ************
traditional_drive::~traditional_drive()
{
    if (!odom_inst) {
        delete odom_inst;
        odom_inst = nullptr;
    }
    // turn off motors
    stop();
};

void traditional_drive::change_drive_mode(int mode) {
    this->mode = mode;
}

// toggle drive mode (arcade, tank, hybrid)
void traditional_drive::toggle_drive_mode()
{
    // 0 = arcade, 1 = tank, 2 = hybrid
    switch (mode)
    {
    case 0:
        arcade_drive(); // call arcade drive
        break;
    case 1:
        tank_drive(); // call tank drive
        break;
    case 2:
        hybrid_drive(); // call hybrid drive
        break;
    default:
        stop(); // stop motors
        break;
    }
};

// operator control tank drive
void traditional_drive::tank_drive()
{
    // get joystick values and apply square scaling
    left *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    right *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y))); // vertical input from right joystick

    setV(); // set voltage to motors
};

// operator control hybrid drive
void traditional_drive::hybrid_drive()
{
    // get joystick values and apply square scaling
    fwd = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y))); // vertical input from right joystick
    // use fwd and turn to calculate voltage to send to motors
    left *= fwd - turn;
    right *= fwd + turn;

    setV(); // set voltage to motors
};

// turn off motors
void traditional_drive::stop()
{
    // set voltage to 0 for both groups
    left = 0;
    right = 0;
    setV();
};

// set voltage to motors
void traditional_drive::setV()
{
    left_side->move_voltage(left);
    right_side->move_voltage(right);
    left = right = scalingFactor; // reset voltage to be multiplied by scalar
};

// operator control arcade drive
void traditional_drive::arcade_drive()
{
    // get joystick values and apply square scaling
    fwd = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));   // vertical input from left joystick
    turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_X))) / 1.7; // horizontal input from right joystick
    // use fwd and turn to calculate voltage to send to motors
    left *= fwd + turn;
    right *= fwd - turn;

    setV(); // set voltage to motors
};


/**
 * @param mag_angle_vector An std::pair of doubles containing (first) the magnitude of the desired
 * movement vector and (second) the angle of the movement in [0, 360) in degrees.
 * To only turn the robot simply pass a magnitude of 0 and the desired angle.
 *
 * @return Moves the robot according to the given std::pair
 */
void traditional_drive::robot_centric_move(pair<double, double> movement_vector)
{
    // rads = deg * pi / 180
    movement_vector.second *= M_PI / 180;

    // x and y components of movement vector
    double x = cos(movement_vector.second) * movement_vector.first; 
    double y = sin(movement_vector.second) * movement_vector.first;

    // determine voltage to send to motors
    left *=(y - x);
    right *=(y + x);

    // send voltage to motors
    setV();
}


/**
 * @param mag_angle_vector An std::pair of doubles containing (first) the magnitude of the desired
 * movement vector and (second) the angle of the movement in [0, 360) in degrees.
 * To only turn the robot simply pass a magnitude of 0 and the desired angle.
 *
 * @return Moves the robot according to the given std::pair
 */
void traditional_drive::field_centric_move(pair<double, double> movement_vector)
{
    // get heading from inertial sensor and subtract from desired angle
    movement_vector.second = movement_vector.second + imu->get_heading() - 360; 

    // call robot centric move with adjusted movement vector
    robot_centric_move(movement_vector);
}


/**
 * Turns the robot on a point.
 *
 * @param power The power to turn with normalized to [-1, 1] where +/- 1 is the maximum turning speed.
 * Positive for clockwise (increasing theta), negative for counterclockwise (decreasing theta).
 *
 * @return Turns the robot with a rotational speed relative to power
 */
void traditional_drive::turn_with_power(double power)
{
    // multiply voltage by power factor
    // left*=power;
    // right=-(std::abs(right)*power);

    left_side->move(power);
    right_side->move(-power);

    // send voltage to motors
    // setV();
}

/**
 * Moves the robot.
 *
 * @param power The power to moves with normalized to [-1, 1] where +/- 1 is the maximum movement speed.
 * Positive for forwards, negative for backwards.
 *
 * @return Moves the robot with a rotational speed relative to power
 */
void traditional_drive::move_with_power(double power)
{
    // multiply voltage by power factor
    // left*=power;
    // right=-(std::abs(right)*power);

    left_side->move(power);
    right_side->move(power);

    // send voltage to motors
    // setV();
}

void traditional_drive::tank_with_power(double latPower, double turnPower)
{
    left_side->move(latPower+turnPower);
    right_side->move(latPower-turnPower);
}

Motor_Group& traditional_drive::get_motor_group(bool side)
{
    if (side == 0)
        return *left_side;
    else
        return *right_side;
}

Imu& traditional_drive::get_imu() {
    return *imu;
}

Controller& traditional_drive::get_controller() {
    return *master;
}

double traditional_drive::getX() {
    if (odom_inst == nullptr) {
        pros::lcd::set_text(3, "trad_drive getX but odom is nullptr!!!");
        return -999;
    }

    return odom_inst->getX();
}

double traditional_drive::getY() {
    if (odom_inst == nullptr) {
        pros::lcd::set_text(3, "trad_drive getY but odom is nullptr!!!");
        return -999;
    }

    return odom_inst->getY();
}

Odom& traditional_drive::getOdom() {
    if (odom_inst == nullptr) {
        pros::lcd::set_text(6, "ERROR: getOdom but it's nullptr!!!");
        pros::delay(100);
        throw std::runtime_error("Called getOdom but odom_inst is null!");
    }

    return *odom_inst;
}

void traditional_drive::brake() {
    left_side->set_brake_modes(BRAKETYPE_BRAKE);
    right_side->set_brake_modes(BRAKETYPE_BRAKE);
    left_side->brake();
    right_side->brake();
}