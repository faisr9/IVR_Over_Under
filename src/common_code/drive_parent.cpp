#include "common_code/drive_parent.h"
#include "pros/adi.hpp"

DriveParent::DriveParent(pros::Imu imu, std::string drive_type): imu_(imu), kDriveType(drive_type) {}

std::string DriveParent::get_drive_type() const {
  return kDriveType;
}

// normalize joystick values to -1 to 1
double DriveParent::normalize_joystick(double input) {
  return input / 127.0;
}
// apply smooth sin scaling 
double DriveParent::sin_scale(double input, double sin_scale_factor) {
  return copysign(pow(sin((3.14159/2) * fabs(input)), sin_scale_factor), input);
}
// apply square scaling
double DriveParent::square_scale(double input) {
  return copysign(pow(input, 2), input);
}