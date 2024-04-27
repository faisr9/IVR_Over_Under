#pragma once
#include "pookie/include_list.h"
#include "pookie/devices.h"

// void auton15();
std::vector<double> vect(double x, double y);
void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor=3.0, double lookAhead=.5, double turnP=2.1);
void turn(double angle);

void main_auton(bool wp);
void win_point_auton();
void non_win_point_auton();