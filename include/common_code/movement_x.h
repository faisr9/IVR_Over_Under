#include "common_code/movement_helper.h"
#include "common_code/x-drive.h"

void followPathX(std::vector<std::vector<double>>& path, x_drive& drive, double finalAngleDeg, bool spinAtEnd = false, bool goal_at_end = false, double lookForwardRadius = 0.5, double final_angle_tolerance_deg = 3.0, double translationalRPM = 200.0, double maxRPM = 450.0, double minTransRPM = 40.0, bool printMessages = false, double turnP = 1.9);
void turnToAngleX(x_drive& drive, double desiredAngleDeg, double toleranceDeg, bool debug = false, double p = 1.9, int time_in_range_millis = 1); // time_in_range_millis set to 1 to be compatible with other branches (for Purdue)
