#include "skills_15/drive.h"

int drive_mode_idx = 0;

std::vector<std::pair<std::string,std::function<void()>>> drive_modes = {{"arc", arcade_drive}};

// normalize joystick values to -1 to 1
double normalize_joystick(double input) {
  return input / 127.0;
}
// apply smooth sin scaling 
double sin_scale(double input, double sin_scale_factor) {
  return copysign(pow(sin((3.14159/2) * fabs(input)), sin_scale_factor), input);
}
// apply square scaling
double square_scale(double input) {
  return copysign(pow(input, 2), input);
}

// operator control drive
void op_drive() {}
// change drive modes
void toggle_drive_mode() {
  drive_mode_idx = (drive_mode_idx + 1) % drive_modes.size();
}
std::string get_drive_name() {
  return drive_modes[drive_mode_idx].first;
}

// Regular tank drive with square scaling
void tank_drive() {
  // double left = square_scale(normalize_joystick(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
  // double right = square_scale(normalize_joystick(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
  // left_side.move_voltage(left * 12000);
  // right_side.move_voltage(right * 12000);
}
// Regular arcade drive with square scaling
void arcade_drive() {
  // double forward = square_scale(normalize_joystick(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
  // double turn = square_scale(normalize_joystick(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)));
  // left_side.move_voltage((forward + turn) * 12000);
  // right_side.move_voltage((forward - turn) * 12000);
}
// Hybrid arcade drive with square scaling
void hybrid_drive() {
  // double forward = square_scale(normalize_joystick(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
  // double turn = square_scale(normalize_joystick(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
  // left_side.move_voltage((forward - turn) * 12000);
  // right_side.move_voltage((forward + turn) * 12000);
}