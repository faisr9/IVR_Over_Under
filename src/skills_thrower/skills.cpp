#include "skills_thrower/skills.h"

void skills() {

    std::vector<double> start_pos = {0, 0};

    x_drive_odom.initTracker(start_pos[0], start_pos[1], 0);
    pros::delay(50);

    std::vector<std::vector<double>> straight_path = {start_pos, {0, 1.0}};

    followPathX(straight_path, xdriveThrower, x_drive_odom, 0);

    // what could possibly go wrong
}