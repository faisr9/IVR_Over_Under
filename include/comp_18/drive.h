#pragma once
#include "main.h"

extern int drive_mode_idx;

void op_drive();
void toggle_drive_mode();
std::string get_drive_name();

void tank_drive();
void arcade_drive();
void hybrid_drive();

extern std::vector<std::pair<std::string,std::function<void()>>> drive_modes;

// Utility functions
double normalize_joysticks(double input);
double sin_scale(double input);
double square_scale(double input);
