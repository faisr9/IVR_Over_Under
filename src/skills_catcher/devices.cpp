#include "skills_catcher/devices.h"

const char LEFT_WING = 'A'; // temp values so the code builds
const char RIGHT_WING = 'A';
const char NET = 'A';

Controller ctrl_master(E_CONTROLLER_MASTER);

Motor front_left(12,1);
Motor front_right(1,1);
Motor back_left(20,0);
Motor back_right(7,0);
Motor left_middle_1(16, GEARSET_06, 0);
Motor left_middle_2(17, GEARSET_06, 0);
Motor right_middle_1(9, GEARSET_06, 0);
Motor right_middle_2(10, GEARSET_06, 0);
Motor_Group straight_left({left_middle_1, left_middle_2});
Motor_Group straight_right({right_middle_1, right_middle_2});

pros::Imu imu(13);
Piston catcher_wings('A');

// Tracking wheels and odom

pros::Rotation radial_rot_sensor(6, true);
pros::Rotation transverse_rot_sensor(8, false);
Generic_Rotation_VEX_Rot* radial_tracker = new Generic_Rotation_VEX_Rot(radial_rot_sensor, 1.96 * 0.0254 / 2, convert::inToM(0.5));
Generic_Rotation_VEX_Rot* transverse_tracker = new Generic_Rotation_VEX_Rot(transverse_rot_sensor, 1.96 * 0.0254 / 2, convert::inToM(5.125));
Odom ast_odom(imu, transverse_tracker, radial_tracker);

asterisk_drive astdriveCatcher(ctrl_master, front_left, front_right, back_left, back_right,straight_right, straight_left, imu);
