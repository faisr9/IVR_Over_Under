#pragma once
#include "api.h"
#include <vector>



class DriveParent {
    // pure virtual probably because doesn't make sense to have undefined drive

    public:
        DriveParent(pros::Imu imu, std::string drive_type);

        // Of course, the child class is free to define its own movement methods too. 
        // These two generic ones are so programs without knowledge of the child drive 
        // type can still move the robot.

        // two methods that move the robot either field centrically or robot centrically

        /**
         * @param mag_angle_vector An std::pair of doubles containing (first) the magnitude of the desired
         * movement vector and (second) the angle of the movement in [0, 360) in degrees
         * 
         * @return Moves the robot according to the given std::pair
        */
        virtual void robot_centric_move(std::pair<double, double> mag_angle_vector) = 0;
        /**
         * @param mag_angle_vector An std::pair of doubles containing (first) the magnitude of the desired
         * movement vector and (second) the angle of the movement in [0, 360) in degrees
         * 
         * @return Moves the robot according to the given std::pair
        */
        virtual void field_centric_move(std::pair<double, double> mag_angle_vector) = 0;

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
};
