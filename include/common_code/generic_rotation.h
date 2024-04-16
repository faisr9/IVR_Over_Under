#pragma once
#include "main.h"

// The purpose of this class is to provide a parent class that can
// leverage polymorphism to generically interact with either a Digikey
// rotation sensor or a VEX rotation sensor

// The generic implementation will consist of returning the change in position in meters

class Generic_Rotation {
    public:
        /**
         * @param offset_from_rot_axis is the distance between the wheel and the axis of rotation where the greater the distance
         * the more the wheel will rotate when the robot spins on a point. This is the line going through the robot that
         * is parallel to the rotation of the wheel. Be careful when measuring this value because the axis isn't always exactly where you think it is.
         * Positive means the wheel is on the front/right side of the robot.
        */
        Generic_Rotation(double wheel_rad_meters, double offset_from_rot_axis_meters = 0.0);
        // must call initialize_sensor before getting the data
        virtual void initialize_sensor() = 0;
        virtual double get_meters_travelled() = 0;
        virtual double get_raw_data() = 0; // directly returns the value of the encoder
        double get_rot_axis_offset() const;

    protected: 
        const double wheel_radius;
        const double offset_from_rot_axis;

    private:
        // helper function that gets the change in rotation in the respective encoder ticks
        virtual double get_delta_rotation() = 0; 
};