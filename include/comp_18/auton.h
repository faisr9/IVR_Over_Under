#pragma once
#include "comp_18/include_list.h"
#include "comp_18/devices.h"

void auton18(double auton_duration_time_millis, bool skills);
std::vector<double> vect(double x, double y);
std::vector<double> vectOff(double x, double y);
void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd);

