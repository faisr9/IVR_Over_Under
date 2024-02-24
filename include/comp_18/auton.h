#pragma once
#include "comp_18/include_list.h"
#include "comp_18/devices.h"

void auton18(double auton_duration_time_millis, bool skills);
void skills18(double auton_duration_time_millis);
std::vector<double> vect(double x, double y);
std::vector<double> vectOff_comp(double x, double y);
std::vector<double> vectOff_skills(double x, double y);
void move_skills(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor=3.0);
void turn_skills(double angle);
void turnF_skills(double angle);
