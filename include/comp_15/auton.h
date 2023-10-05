#pragma once
#include "api.h"

void drivePID(double inches);
double getAngleError(double target, double currHeading);
void turnPID(double deg, double kp, double ki, double kd, double max_speed, double min_speed);
void auton_thread();
void auton();
