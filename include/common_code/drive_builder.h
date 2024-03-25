#pragma once
#include "main.h"

class drive_builder {
    private:
        pros::Controller *drive_controller;

        pros::Motor_Group *left_drive;
        pros::Motor_Group *right_drive;
        using motor = pros::Motor;
        
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
         * | L-mtr |            | R-mtr |
         * |___2___|============|___2___|
         * | L-mtr |============| R-mtr |
         * |___3___|            |___3___|
         * | L-mtr |            | R-mtr |
         * |___4___|            |___4___|
         * |============================|
         * 
         * ==============================================
         * Holonomic Drive Motor Order:
         * - 4 motor holonomic drive will use motors 1 and 2
         * - 6 motor holonomic drive will use motors 1, 2, 3
         * - 8 motor holonomic drive will use motors 1, 2, 3, 4 in pairs
         * 
         *            Front
         * - inerst holonomic drive diagram here
        */
        // motor right_1;
        // motor right_2;
        // motor right_3;
        // motor right_4;
        // motor left_1;
        // motor left_2;
        // motor left_3;
        // motor left_4;
        // motor strafe;
        motor *drive_motors;

        // Drive configurations
        typedef enum drive_config_e {
            TANK,
            HDRIVE,
            HOLOMOIC,
            XDRIVE=HOLOMOIC,
            CUSTOM
        } drive_config;

        typedef enum drive_mode_e {
            TANK,
            SINGLE_STICK_ARCADE_R,
            SINGLE_STICK_ARCADE_L,
            SPLIT_ARCADE_PR,
            SPLIT_ARCADE_PL,
            HOLOMOIC_SR=SPLIT_ARCADE_PR,
            HOLOMOIC_SL=SPLIT_ARCADE_PL,
            CUSTOM
        } drive_mode;

        // All scaling factors and other variables
        void square_scale(double input);
        void sin_scale(double input, double sin_scale_factor);

        // Drive config methods
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

    public:
        drive_builder(drive_config_e config, pros::Controller *controller);

        drive_builder &with_motors(pros::Motor_Group *left, pros::Motor_Group *right);
        drive_builder &with_motors(motor *left_1, motor *left_2, motor *right_1, motor *right_2);
        drive_builder &with_motors(motor *left_1, motor *left_2, motor *left_3, motor *right_1, motor *right_2, motor *right_3);
        drive_builder &with_motors(motor *left_1, motor *left_2, motor *left_3, motor *left_4, motor *right_1, motor *right_2, motor *right_3, motor *right_4);
        
        drive_builder &add_strafe_motor(motor *strafe);
        
        drive_builder &set_default_drive_mode(drive_mode_e mode);
        drive_builder &init();

        // TBD Scaling factors
        // drive_builder &use_square_scaling();
        // drive_builder &use_sin_scaling(double sin_scale_factor);

        void start_drive();
        void pause_drive();
        void stop_drive();

        void test_drive();

        void set_drive_mode(drive_mode_e mode);
        // void set_(/* all scaling factors */);
        // void set_other_variables(/* all other variables */);
};