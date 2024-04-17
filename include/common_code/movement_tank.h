#pragma once
#include "main.h"
#include "common_code/movement_helper.h"

// void followPath(std::vector<std::vector<double>>& path, double lookForwardRadius, double translationalRPM, double maxRPM, double finalAngleDeg, bool reversed, bool printMessages);
void followPath(std::vector<std::vector<double>>& path, traditional_drive& drive, double finalAngleDeg, bool reversed, bool spinAtEnd = false, bool goal_at_end = false, double lookForwardRadius = 0.5, double final_angle_tolerance_deg = 3.0, double translationalRPM = 200.0, double maxRPM = 450.0, double minTransRPM = 40.0, bool printMessages = false, double turnP = 1.9);
void turnPID(traditional_drive& drive, double desiredAngleDeg, double toleranceDeg=.2, double p=2.6, double i=0, double d=2.2);
void latPID(traditional_drive& drive, double target, double tolerance, double p=0.1, double i=0, double d=0);
void movePID(traditional_drive& drive, double target, double angle, double latTolerance, double turnTolerance, double lP=.1, double lI=0.0, double lD=0.0,double tP=2.6, double tI=0.0, double tD=2.2);
void turnToAngle(traditional_drive& drive, double desiredAngleDeg, double toleranceDeg, bool debug = false, double p = 1.9, int time_in_range_millis = 1); // time_in_range_millis set to 1 to be compatible with other branches (for Purdue)
// Calculate the angle needed to face the goal when at a given (future) position.
void turnToPoint(traditional_drive& drive, double turnP = 1.9, double goalXCoordinate = 0.0, double goalYCoordinate = 0.0);
void moveMotors(traditional_drive& drive, double leftRPM, double rightRPM);
void stopMotors(traditional_drive& drive);