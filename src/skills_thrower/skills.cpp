#include "skills_thrower/skills.h"

void skills() {

    std::vector<double> start_pos = {0, 0};

    x_drive_odom.initTracker(start_pos[0], start_pos[1], 0);

    std::vector<std::vector<double>> straight_path = {start_pos, {0, 1.0}};

    
}