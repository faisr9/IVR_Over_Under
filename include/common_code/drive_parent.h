#pragma once
#include "api.h"
#include <vector>



class DriveParent {
    // pure virtual probably because doesn't make sense to have undefined drive

    public:
        DriveParent(pros::Imu imu, std::string drive_type);
        // public implementation is up to child class. It should supply methods that
        // take user input and convert it to the two private methods
        std::string get_drive_type() const;

        // normalize joystick values to -1 to 1
        static double normalize_joystick(double input);
        // apply smooth sin scaling 
        static double sin_scale(double input, double sin_scale_factor);
        // apply square scaling
        static double square_scale(double input);
        
    private:
        // motors stored at child level
        pros::Imu imu_;
        const std::string kDriveType;

        // two methods that move the robot either field centrically or robot centrically
        virtual void robot_centric_move(std::pair<double, double> movement_vector) = 0;
        virtual void field_centric_move(std::pair<double, double> movement_vector) = 0;

};
