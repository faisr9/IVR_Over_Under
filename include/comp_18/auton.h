#pragma once
#include "comp_18/include_list.h"
#include "comp_18/devices.h"

void skills18(double auton_duration_time_millis);
std::vector<double> vect(double x, double y);
std::vector<double> vectOffComp(double x, double y);
std::vector<double> vectOffSkills(double x, double y);
void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor=3.0);
void turn(double angle);
void turnF(double angle);
