#pragma once
#include <vector>

// void followPath(std::vector<std::vector<double>>& path, double lookForwardRadius, double translationalRPM, double maxRPM, double finalAngleDeg, bool reversed, bool printMessages);
void followPath(std::vector<std::vector<double>>& path, double finalAngleDeg, bool reversed, bool spinAtEnd = false, bool goal_at_end = false, double lookForwardRadius = 0.5, double final_angle_tolerance_deg = 3.0, double translationalRPM = 200.0, double maxRPM = 450.0, double minTransRPM = 40.0, bool printMessages = false); // 200.0, 450.0
void turnToAngle(double desiredAngleDeg, double toleranceDeg, bool debug = false, double p = 3.0);
// Calculate the angle needed to face the goal when at a given (future) position.
void turnToPoint(double goalXCoordinate = 0.0, double goalYCoordinate = 0.0);
void moveMotors(double leftRPM, double rightRPM);
void SmartStop();
void stopMotors();