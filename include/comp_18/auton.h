#pragma once
#include "common_code/include_list.h"

void auton18(double auton_duration_time_millis);
std::vector<double> vect(double x, double y);
std::vector<double> vectOff(double x, double y);
void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd);

