#pragma once
#include "main.h"

void auton18();
std::vector<double> vect(double x, double y);
std::vector<double> vectOff(double x, double y);
void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd);