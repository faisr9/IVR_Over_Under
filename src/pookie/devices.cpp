#include "comp_15/devices.h"
#include "comp_15/include_list.h"

using namespace pros;

/**
 * Future Update:
 * When the graphics interface is implemented, a devices class will be used to manage
 * all of the devices on the robot. This will allow the port changer on the robot to
 * be active incase a port is damaged.
*/

pros::Controller ctrl_master(E_CONTROLLER_MASTER);

// Motors //
// front is where intake is
//12 13 15 ports are cooked 
pros::Motor right_front(11,pros::motor_gearset_e::E_MOTOR_GEAR_BLUE, true);
pros::Motor right_front_middle(13,pros::motor_gearset_e::E_MOTOR_GEAR_BLUE, false);
pros::Motor right_back_middle(14,pros::motor_gearset_e::E_MOTOR_GEAR_BLUE, true);
pros::Motor right_back(15,pros::motor_gearset_e::E_MOTOR_GEAR_BLUE, false);

pros::Motor left_front(1,pros::motor_gearset_e::E_MOTOR_GEAR_BLUE,false);
pros::Motor left_front_middle(3,pros::motor_gearset_e::E_MOTOR_GEAR_BLUE, true);
pros::Motor left_back_middle(4,pros::motor_gearset_e::E_MOTOR_GEAR_BLUE, false);
pros::Motor left_back(5,pros::motor_gearset_e::E_MOTOR_GEAR_BLUE, true);

pros::Motor_Group left_drive_motors = {left_front, left_front_middle, left_back_middle, left_back};
pros::Motor_Group right_drive_motors = {right_front, right_front_middle, right_back_middle, right_back};

pros::Motor intake(10); // in = negative, out = positive
Intake* intake_instance = Intake::createInstance(intake);

// V5 Sensors //
pros::IMU imu(21);
pros::Distance distance_sensor(3);
pros::Rotation radial_rot_sensor(10);
pros::Rotation transverse_rot_sensor(9, true); // reversed so that going right is positive

Generic_Rotation* radial_tracker = new Generic_Rotation_VEX_Rot(radial_rot_sensor, 1.96 * 0.0254 / 2, 0.013);
Generic_Rotation* horizontal_tracker = new Generic_Rotation_VEX_Rot(transverse_rot_sensor, 1.96 * 0.0254 / 2, -0.168);

Odom odometry_15(imu, horizontal_tracker, radial_tracker);

const char WINGS = 'B';
const char SIDEHANG = 'C';
const char INTAKE = 'D';
const char TOPHANG = 'A';
Pneumatics* pneumatics_instance = Pneumatics::createInstance(WINGS, SIDEHANG, TOPHANG, INTAKE);

// Other //
traditional_drive tank_drive_15(imu, ctrl_master, left_drive_motors, right_drive_motors, odometry_15);

// Legacy Sensors //

// Distance Sensors //
const int kTRIBALL_DETECTION_DIST = 100;
bool triBall()
{
    // if the distance sensor detects something within 100mm
    return (distance_sensor.get() < kTRIBALL_DETECTION_DIST);
}
