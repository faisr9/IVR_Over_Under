#include "common_code/generic_rotation_digikey.h"


Generic_Rotation_Digikey::Generic_Rotation_Digikey(pros::ADIEncoder& rot_sensor, double wheel_radius_meters): Generic_Rotation(wheel_radius_meters), rotation_sensor(rot_sensor) {}

void Generic_Rotation_Digikey::initialize_sensor() {
    last_value = rotation_sensor.get_value();
}

double Generic_Rotation_Digikey::get_meters_travelled() {
    return 2 * M_PI * wheel_radius * (get_delta_rotation() / kTICKS_PER_REVOLUTION);
}

double Generic_Rotation_Digikey::get_delta_rotation() {
    double current_position = rotation_sensor.get_value();

    double rotation_difference = current_position - last_value;

    last_value = current_position;
    return rotation_difference;
}

