#pragma once
#include "main.h"
#include "common_code/generic_rotation.h"


class Generic_Rotation_VEX_Rot : public Generic_Rotation {
    public:
        Generic_Rotation_VEX_Rot(pros::Rotation& rot_sensor, double wheel_radius);
        double get_meters_travelled();
        void initialize_sensor();
    private:
        pros::Rotation& rotation_sensor;
        double get_delta_rotation(); 
        double last_value;
        const double kTICKS_PER_REVOLUTION = 36000.0;

};