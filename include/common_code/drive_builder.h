/**
 * Things to do:
 * - Finish driveClass to run all code
 * - Drive builder to build driveClass only
 * - move methods from drive builder to driveClass
 * - double check pointers and references
*/


#pragma once
#include "main.h"

using motor = pros::Motor;
using motor_g = pros::Motor_Group;

class driveClass {
    public:
        driveClass (
            motor_g *left, motor_g *right, pros::Controller *controller,
            bool isSquareScaling=false, bool isSinScaling=false, bool isAccelScaling=false,
            double sin_scale_factor=-999, double accel_scale_factor=-999,
            double left_scale=-999, double right_scale=-999,
            int left_deadzone=-999, int right_deadzone=-999
        );

        driveClass (
            std::vector<motor*> *drive_motors, pros::Controller *controller,
            bool isSquareScaling=false, bool isSinScaling=false, bool isAccelScaling=false,
            double sin_scale_factor=-999, double accel_scale_factor=-999,
            double left_scale=-999, double right_scale=-999,
            int left_deadzone=-999, int right_deadzone=-999
        );

    private:
        motor_g *left_drive;
        motor_g *right_drive;
        motor *strafe_motor;
        std::vector<motor*> drive_motors;
        pros::Controller *drive_controller;

        void tank();
        void hdrive();
        void holonomic_drive();
        void xdrive();
        void custom_drive();

        // Drive mode methods
        void tank_drive();
        void single_stick_arcade_right();
        void single_stick_arcade_left();
        void split_arcade_right();
        void split_arcade_left();
        void custom_drive_mode();

        // Drive config methods
        void tank();
        void hdrive();
        void holonomic_drive();
        void xdrive();
        void custom_drive();

        // All scaling factors and other variables
        void square_scale(double input);
        void sin_scale(double input, double sin_scale_factor);
};

class drive_builder {
    public:
        typedef enum drive_mode_e {
            TANK,
            SINGLE_STICK_ARCADE_R,
            SINGLE_STICK_ARCADE_L,
            SPLIT_ARCADE_PR,
            SPLIT_ARCADE_PL,
            HDRIVE_TANK,
            HOLONOMIC_SR,
            HOLONOMIC_SL,
            CUSTOM
        };

        typedef enum drive_config_e {
            TANK,
            HDRIVE,
            HOLONOMIC,
            XDRIVE=HOLONOMIC,
            CUSTOM
        };

        drive_builder(drive_config_e config, pros::Controller *controller);

        drive_builder &with_motors(motor_g *left, motor_g *right);
        drive_builder &with_motors(motor *left_1, motor *left_2, motor *right_1, motor *right_2, motor *strafe=nullptr);
        drive_builder &with_motors(motor *left_1, motor *left_2, motor *left_3, motor *right_1, motor *right_2, motor *right_3, motor *strafe=nullptr);
        drive_builder &with_motors(motor *left_1, motor *left_2, motor *left_3, motor *left_4, motor *right_1, motor *right_2, motor *right_3, motor *right_4, motor *strafe=nullptr);
        
        drive_builder &set_default_drive_mode(drive_mode_e mode);
        drive_builder &add_ctrl_deadzone(int left_deadzone, int right_deadzone);
        drive_builder &add_straight_drive_scale(double left_scale, double right_scale);
        drive_builder &use_square_scaling();
        drive_builder &use_sin_scaling(double sin_scale_factor);
        drive_builder &use_acceleration_scaling(double accel_scale_factor);
        drive_builder &init();

        // TBD Scaling factors

        void start_drive();
        void pause_drive();
        void stop_drive();

        void test_drive();

        void set_drive_mode(drive_mode_e mode);
        // void set_(/* all scaling factors */);
        // void set_other_variables(/* all other variables */);
    private:        
        /**
         * Tank Drive Motor Order:
         * - 4 motor tank drive will use motors 1 and 2
         * - 6 motor tank drive will use motors 1, 2, 3
         * - 8 motor tank drive will use motors 1, 2, 3, 4
         * 
         *             Front
         * |_______              _______| 
         * | L-mtr |            | R-mtr |
         * |___1___|            |___1___|
         * | L-mtr |============| R-mtr |
         * |___2___| Strafe mtr |___2___|
         * | L-mtr |   H-Drive  | R-mtr |
         * |___3___|============|___3___|
         * | L-mtr |            | R-mtr |
         * |___4___|            |___4___|
         * |============================|
         * 
         * ==============================================
         * Holonomic Drive Motor Order:
         * - 4 motor holonomic drive will use motors 1 and 2
         * - 6 motor holonomic drive will use motors 1, 2, 3
         * 
         *     Front (Same with Right Side)
         * 
         *      _//       _/
         *   _// L-mtr 1_/    
         * //        /       
         * ||        |       
         * ||        |       
         * ||  Left  |     
         * ||  mtr 2 |       
         * ||        |       
         * ||        |       
         * \\_       \_      
         *    \\_L-mtr 3\_   
         *       \\        \ 
         *  
        */

        pros::Controller *drive_controller;

        motor_g *left_drive;
        motor_g *right_drive;

        std::vector<motor*> drive_motors;
        motor *strafe_motor;

        bool isSquareScaling, isSinScaling, isAccelScaling;
        double sin_scale_factor, accel_scale_factor;
        double left_scale, right_scale;
        int left_deadzone, right_deadzone;
        int checksum;

        // Drive configurations
        drive_config_e drive_config;
        drive_mode_e drive_mode;
};