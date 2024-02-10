#pragma once

// #define M_PI 3.14159265358979323846

#include "api.h" // DO NOT REMOVE //

#include <iostream>
#include <chrono>
#include <string>
#include <math.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

#include "common_code/subsystem_parent.h"

// Vex defined function, might as well have just in case
#define waitUntil(condition)                                                   \
  do {                                                                         \
    pros::delay(5);                                                            \
  } while (!(condition))

// using namespace pros;
// using namespace std;