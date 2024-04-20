#include "skills_thrower/devices.h"
#include "skills_thrower/include_list.h"

pros::Controller ctrl_master(E_CONTROLLER_MASTER);

// SkillsCata* cata_inst = SkillsCata::createInstance();

pros::Motor front_left(12, GEARSET_18, true);
pros::Motor front_right(15, GEARSET_18, true);
pros::Motor back_left(10, GEARSET_18, false);
pros::Motor back_right(1, GEARSET_18, true);

pros::Imu imu(18);

x_drive xdriveThrower(ctrl_master, front_left, front_right, back_left, back_right, imu);

Rotation radial_rot_sensor(8, true); // increasing angle going forward = don't reverse
Rotation transverse_rot_sensor(14, false);

Generic_Rotation_VEX_Rot radial_tracker(radial_rot_sensor, 1.96 * 0.0254 / 2.0, 0);
Generic_Rotation_VEX_Rot transverse_tracker(transverse_rot_sensor, 1.96 * 0.0254 / 2.0, 5.33 * 0.0254);

Odom x_drive_odom(imu, &transverse_tracker, &radial_tracker);