#pragma once


// The purpose of this class is to provide a parent class that can
// leverage polymorphism to generically interact with either a Digikey
// rotation sensor or a VEX rotation sensor

// The generic implementation will consist of returning the change in position in meters

class Generic_Rotation {
    public:
        Generic_Rotation(double wheel_rad_meters);
        // must call initialize_sensor before getting the data
        virtual void initialize_sensor() = 0;
        virtual double get_meters_travelled() = 0;
        virtual double get_raw_data() = 0; // directly returns the value of the encoder

    protected: 
        const double wheel_radius;

    private:
        // helper function that gets the change in rotation in the respective encoder ticks
        virtual double get_delta_rotation() = 0; 
};