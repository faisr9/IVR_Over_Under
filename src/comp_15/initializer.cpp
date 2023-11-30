#include "comp_15/comp15_includeList.h"

// pros::MotorGroup Cata   ({cata_right, cata_left});
// pros::ADIButton     Cata_limit  ('A');

int cata_load_voltage = 14;
int cata_launch_voltage = 14;
// CompetitionCatapult::createInstance(Cata, Cata_limit, cata_load_voltage, cata_launch_voltage);
CompetitionCatapult::getInstance();