#include "common_code/generic_rotation.h"

Generic_Rotation::Generic_Rotation(double wheel_rad_meters, double offset_from_rot_axis_meters): wheel_radius(wheel_rad_meters), offset_from_rot_axis(offset_from_rot_axis_meters) {}

double Generic_Rotation::get_rot_axis_offset() const {
    return offset_from_rot_axis;
}