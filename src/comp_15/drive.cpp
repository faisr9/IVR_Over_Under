#include "comp_15/drive.h"

int drive_mode_idx = 0;

std::vector<std::pair<std::string,std::function<void()>>> drive_modes = {{"dyl", dylan_drive}, {"aka", akap_drive}, {"arc", arcade_drive}};

// normalize joystick values to -1 to 1
double normalize_joystick(double input) {
}
// apply smooth sin scaling 
double sin_scale(double input, double sin_scale_factor) {
}
// apply square scaling
double square_scale(double input) {
}

// operator control drive
void op_drive() {
}
// change drive modes
void toggle_drive_mode() {
}
std::string get_drive_name() {
}


// Dylan's Drive
const double dylan_sin_scale_factor = 2.9;
void dylan_drive() {
}
// Akap.site's Drive
const double akap_sin_scale_factor = 2.5;
void akap_drive() {
}

// Regular tank drive with square scaling
void tank_drive() {
}
// Regular arcade drive with square scaling
void arcade_drive() {
}
// Hybrid arcade drive with square scaling
void hybrid_drive() {
}