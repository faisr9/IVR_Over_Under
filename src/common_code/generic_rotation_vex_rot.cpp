#include "common_code/generic_rotation_vex_rot.h"


Generic_Rotation_VEX_Rot::Generic_Rotation_VEX_Rot(pros::Rotation& rot_sensor, double wheel_radius_meters, double offset_from_rot_axis_meters): Generic_Rotation(wheel_radius_meters, offset_from_rot_axis_meters), rotation_sensor(rot_sensor) {}

void Generic_Rotation_VEX_Rot::initialize_sensor() {
    rotation_sensor.set_position(0);
    last_value = rotation_sensor.get_position();
}

double Generic_Rotation_VEX_Rot::get_meters_travelled() {
    return 2 * M_PI * wheel_radius * (get_delta_rotation() / kTICKS_PER_REVOLUTION);
}

double Generic_Rotation_VEX_Rot::get_delta_rotation() {
    double current_position = rotation_sensor.get_position();
    //pros::lcd::set_text(4, "Current angle is " + std::to_string(current_position));

    double rotation_difference = current_position - last_value;
    
    // despite what the docs say, the value returned by get_position does not rollover from
    // 36000 to 0 (it just keeps going up), so rollover detection/accomodation code
    // is not needed

    last_value = current_position;
    return rotation_difference;
}

double Generic_Rotation_VEX_Rot::get_raw_data() {
    return rotation_sensor.get_position();
}