#pragma once
#include "main.h"

using motor = pros::Motor;
using motor_g = pros::Motor_Group;

class driveClass {
    private:
    public:
        class driveBuilder;
        enum drive_mode_e{
            TANK_m,
            SINGLE_STICK_ARCADE_R,
            SINGLE_STICK_ARCADE_L,
            SPLIT_ARCADE_PR,
            SPLIT_ARCADE_PL,
            HDRIVE_TANK,
            HOLONOMIC_SR,
            HOLONOMIC_SL,
            CUSTOM_m
        };

        enum drive_config_e{
            TANK_c,
            HDRIVE,
            HOLONOMIC,
            XDRIVE=HOLONOMIC,
            CUSTOM_c
        };

        static driveBuilder buildDrive (drive_config_e config, pros::Controller *controller);

        // Drive control methods
        void start_drive();
        void pause_drive();
        void stop_drive();

        void test_drive();

        void set_drive_mode(drive_mode_e mode);

    private:        
        pros::Controller *drive_ctrler;

        motor_g *left_drive;
        motor_g *right_drive;

        std::vector<motor*> drive_motors;
        motor *strafe_motor;

        bool isSquareScaling, isSinScaling, isAccelScaling;
        double sin_scale_factor, accel_scale_factor;
        double left_scale, right_scale;
        int left_deadzone, right_deadzone;
        int checksum;
        
        // Drive control variables
        int left_stick, right_stick;
        int right, left, strafe, turn, fwd;

        // Drive configurations
        drive_config_e drive_config;
        drive_mode_e drive_mode;

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

        // All scaling factors and other variables
        double square_scale(double input);
        double sin_scale(double input);
        double accel_scale(double input);
        double normalize_joystick(double input);

        // Movement methods
        void setMotors();

        friend class driveBuilder;
};

class driveClass::driveBuilder {
    public:
        driveBuilder(drive_config_e config, pros::Controller *controller);
        driveBuilder &with_motors(motor_g *left, motor_g *right);
        driveBuilder &with_motors(motor *left_1, motor *left_2, motor *right_1, motor *right_2, motor *strafe=nullptr);
        driveBuilder &with_motors(motor *left_1, motor *left_2, motor *left_3, motor *right_1, motor *right_2, motor *right_3, motor *strafe=nullptr);
        driveBuilder &with_motors(motor *left_1, motor *left_2, motor *left_3, motor *left_4, motor *right_1, motor *right_2, motor *right_3, motor *right_4, motor *strafe=nullptr);
        
        driveBuilder &set_default_drive_mode(drive_mode_e mode);
        driveBuilder &add_ctrl_deadzone(int left_deadzone, int right_deadzone);
        driveBuilder &add_straight_drive_scale(double left_scale, double right_scale);
        driveBuilder &use_square_scaling();
        driveBuilder &use_sin_scaling(double sin_scale_factor);
        driveBuilder &use_acceleration_scaling(double accel_scale_factor);
        driveClass init();
    private:
        driveClass builder_obj;
};

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