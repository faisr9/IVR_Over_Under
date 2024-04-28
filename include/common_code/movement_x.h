#include "common_code/movement_helper.h"
#include "common_code/x-drive.h"

const double kDEFAULT_P = 2.8;
void followPathX(std::vector<std::vector<double>>& path, x_drive& drive, Odom& odom, double finalAngleDeg, bool spinAtEnd = true, bool goal_at_end = false, double lookForwardRadius = 0.25, double final_angle_tolerance_deg = 3.0, double translationalRPM = 200.0, double maxRPM = 200.0, double minTransRPM = 20.0, bool printMessages = false, double turnP = kDEFAULT_P);
void turnToAngleX(x_drive& drive, Odom& odom, double desiredAngleDeg, double toleranceDeg, bool debug = false, double p = kDEFAULT_P, int time_in_range_millis = 1); // time_in_range_millis set to 1 to be compatible with other branches (for Purdue)
