#pragma once
#include "main.h"
#include "common_code/generic_rotation.h"


class Generic_Rotation_Digikey : public Generic_Rotation {
    public:
        Generic_Rotation_Digikey(pros::ADIEncoder& rot_sensor, double wheel_radius);
        double get_meters_travelled();
        double get_raw_data();
        void initialize_sensor();
    private:
        pros::ADIEncoder& rotation_sensor;
        double get_delta_rotation(); 
        double last_value;
        const double kTICKS_PER_REVOLUTION = 2000.0; // not tested, will vary based on hardware option

};