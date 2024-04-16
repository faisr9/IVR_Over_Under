#pragma once
#include "main.h"
#include "common_code/movement_helper.h"

// void followPath(std::vector<std::vector<double>>& path, double lookForwardRadius, double translationalRPM, double maxRPM, double finalAngleDeg, bool reversed, bool printMessages);
void followPath(std::vector<std::vector<double>>& path, traditional_drive& drive, double finalAngleDeg, bool reversed, bool spinAtEnd = false, bool goal_at_end = false, double lookForwardRadius = 0.5, double final_angle_tolerance_deg = 3.0, double translationalRPM = 200.0, double maxRPM = 450.0, double minTransRPM = 40.0, bool printMessages = false, double turnP = 1.9);
void turnPID(traditional_drive& drive, double desiredAngleDeg, double toleranceDeg, double p=.1, double i=0, double d=0);
void turnToAngle(traditional_drive& drive, double desiredAngleDeg, double toleranceDeg, bool debug = false, double p = 1.9, int time_in_range_millis = 1); // time_in_range_millis set to 1 to be compatible with other branches (for Purdue)
// Calculate the angle needed to face the goal when at a given (future) position.
void turnToPoint(traditional_drive& drive, double turnP = 1.9, double goalXCoordinate = 0.0, double goalYCoordinate = 0.0);
void moveMotors(traditional_drive& drive, double leftRPM, double rightRPM);
void stopMotors(traditional_drive& drive);