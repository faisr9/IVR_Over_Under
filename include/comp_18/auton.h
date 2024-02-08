#pragma once
#include "main.h"

void auton18();
std::vector<double> vect(double x, double y, double numTiles);
void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd);