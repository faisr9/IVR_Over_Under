#include "skills_thrower/devices.h"
#include "skills_thrower/include_list.h"

pros::Controller ctrl_master(E_CONTROLLER_MASTER);

// SkillsCata* cata_inst = SkillsCata::createInstance();

pros::Motor front_left(1, GEARSET_18, true);
pros::Motor front_right(10, GEARSET_18, true);
pros::Motor back_left(11, GEARSET_18, false);
pros::Motor back_right(17, GEARSET_18, false);

pros::Imu imu(9);

x_drive xdriveThrower(ctrl_master, front_left, front_right, back_left, back_right, imu);

Rotation radial_rot_sensor(4, false); // increasing angle going forward = don't reverse
Rotation transverse_rot_sensor(18, true);

Generic_Rotation_VEX_Rot radial_tracker(radial_rot_sensor, 1.96 * 0.0254 / 2.0, convert::inToM(-4.875)); // need to do offset measure
Generic_Rotation_VEX_Rot transverse_tracker(transverse_rot_sensor, 1.96 * 0.0254 / 2.0, 0.0); // need to do offset measure

Odom x_drive_odom(imu, &transverse_tracker, &radial_tracker);

pros::Motor cata_left(12, true); // spin forward to cycle
pros::Motor cata_right(14, false); // spin forward to cycle cata
pros::Motor_Group cata_motors = {cata_left, cata_right};
pros::Rotation cata_rot_sensor(15, false);
SkillsCata* cata_inst = SkillsCata::createInstance(cata_motors, cata_rot_sensor);
