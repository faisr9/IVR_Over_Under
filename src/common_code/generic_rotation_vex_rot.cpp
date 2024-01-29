#include "common_code/generic_rotation_vex_rot.h"


Generic_Rotation_VEX_Rot::Generic_Rotation_VEX_Rot(pros::Rotation& rot_sensor, double wheel_radius_meters): Generic_Rotation(wheel_radius_meters), rotation_sensor(rot_sensor) {}

void Generic_Rotation_VEX_Rot::initialize_sensor() {
    rotation_sensor.set_position(0);
    last_value = rotation_sensor.get_position();
}

double Generic_Rotation_VEX_Rot::get_meters_travelled() {
    return 2 * M_PI * wheel_radius * (get_delta_rotation() / kTICKS_PER_REVOLUTION);
}

double Generic_Rotation_VEX_Rot::get_delta_rotation() {
    double current_position = rotation_sensor.get_position();

    double rotation_difference = current_position - last_value;

    last_value = current_position;
    return rotation_difference;
}

