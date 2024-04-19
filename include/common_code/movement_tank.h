#pragma once
#include "main.h"
#include "common_code/movement_helper.h"

// void followPath(std::vector<std::vector<double>>& path, double lookForwardRadius, double translationalRPM, double maxRPM, double finalAngleDeg, bool reversed, bool printMessages);
void followPath(std::vector<std::vector<double>>& path, traditional_drive& drive, double finalAngleDeg, bool reversed, bool spinAtEnd = false, bool goal_at_end = false, double lookForwardRadius = 0.5, double final_angle_tolerance_deg = 3.0, double translationalRPM = 200.0, double maxRPM = 450.0, double minTransRPM = 40.0, bool printMessages = false, double turnP = 1.9);
void turnPID(traditional_drive& drive, double desiredAngleDeg, int maxTime=2000, double toleranceDeg=.4, double p=.908, double i=0.000001, double d=1.15);
void latPID(traditional_drive& drive, double target, int maxTime=2500, double tolerance=0.8, double p=0.1, double i=0, double d=0);
void movePID(traditional_drive& drive, double target, double angle, int maxTime=2000, double latTolerance=1.2, double turnTolerance=.2, double lP=3.75, double lI=0.0, double lD=2,double tP=.908, double tI=0.000001, double tD=1.15);
void turnToAngle(traditional_drive& drive, double desiredAngleDeg, double toleranceDeg, bool debug = false, double p = 2.1, int time_in_range_millis = 1); // time_in_range_millis set to 1 to be compatible with other branches (for Purdue)
// Calculate the angle needed to face the goal when at a given (future) position.
void turnToPoint(traditional_drive& drive, double turnP = 1.9, double goalXCoordinate = 0.0, double goalYCoordinate = 0.0);
void moveMotors(traditional_drive& drive, double leftRPM, double rightRPM);
void stopMotors(traditional_drive& drive);