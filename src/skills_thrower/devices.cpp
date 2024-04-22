#include "skills_thrower/devices.h"

Controller ctrl_master(E_CONTROLLER_MASTER);

Motor front_left(12,1);
Motor front_right(15,1);
Motor back_left(10,0);
Motor back_right(1,0);


Imu imu(18);


x_drive xdriveThrower(ctrl_master, front_left, front_right, back_left, back_right,imu);