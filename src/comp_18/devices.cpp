#include "comp_18/comp18_includeList.h"

/**
 * Future Update:
 * When the graphics interface is implemented, a devices class will be used to manage
 * all of the devices on the robot. This will allow the port changer on the robot to
 * be active incase a port is damaged.
*/

pros::Controller ctrl_master(E_CONTROLLER_MASTER);
// Motors //
// front is where intake is
pros::Motor right_front_top(14, true); // 14
pros::Motor right_front_bottom(11, false);
pros::Motor right_middle(12, false); // might actually be port 13
pros::Motor right_back(13, false); // might actually be port 12

pros::Motor left_front_top(17, false); // 14
pros::Motor left_front_bottom(18, true); // one of 18, 19, or 20
pros::Motor left_middle(19, true); // one of 18, 19, or 20
pros::Motor left_back(20, true); // one of 18, 19, or 20

pros::Motor_Group left_drive_motors = {left_front_top, left_front_bottom, left_middle, left_back};
pros::Motor_Group right_drive_motors = {right_front_top, right_front_bottom, right_middle, right_back};

pros::Motor intake(15); // in = negative, out = positive

// V5 Sensors //
pros::IMU imu(2);
pros::Distance distance_sensor(3);
pros::Rotation radial_rot_sensor(10);
pros::Rotation transverse_rot_sensor(9, true); // reversed so that going right is positive
const char WINGS_PORT = 'A';
const char CLIMBER_SOLENOID_PORT = 'B';

// Other //
traditional_drive tank_drive_18(imu, ctrl_master, left_drive_motors, right_drive_motors, 0);

// Legacy Sensors //