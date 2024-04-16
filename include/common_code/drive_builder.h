/**
 * Things to do:
 * Allow custom drive mode regardless of drive configuration
 * Force custom drive mode to be used if drive configuration is set to custom
 * allow use of only 1 scale factor type
 * use binary checksum
*/

#pragma once
#include "main.h"

using motor = pros::Motor;
using motor_g = pros::Motor_Group;

class driveClass {
    private:
    public:
        class driveBuilder;
        typedef enum {
            TANK_m = 1,
            SINGLE_STICK_ARCADE_R,
            SINGLE_STICK_ARCADE_L,
            SPLIT_ARCADE_PR,
            SPLIT_ARCADE_PL,
            HDRIVE_TANK,
            HOLONOMIC_SR,
            HOLONOMIC_SL,
            CUSTOM_m
        } drive_mode_e;

        typedef enum {
            TANK_c = 1,
            HDRIVE,
            HOLONOMIC,
            XDRIVE=HOLONOMIC,
            CUSTOM_c
        } drive_config_e;

        static driveBuilder buildDrive (drive_config_e config, pros::Controller *controller);

        // Drive control methods
        void start_drive();
        void pause_drive();
        void stop_drive();

        void test_drive();

        void set_drive_mode(drive_mode_e mode, bool resume=true);

    private:
        static driveClass *drive_instance_;
        pros::Controller *drive_ctrler;

        motor_g *left_drive;
        motor_g *right_drive;

        std::vector<motor*> drive_motors;
        motor *strafe_motor;

        bool isSquareScaling, isSinScaling;
        double sin_scale_factor;
        double left_scale, right_scale;
        int left_deadzone, right_deadzone;
        uint8_t checksum;
        bool runDriveLoop;
        
        // Drive control variables
        struct ctrler_axis_s {
            int l_x, l_y, r_x, r_y;
        };
        ctrler_axis_s raw_axis;
        ctrler_axis_s calc_axis;
        int right, left, strafe, turn, fwd;
        const int motorPower = 12000; // Scalar: current scale is in millivolts

        // Drive configurations
        drive_config_e drive_config;
        drive_mode_e drive_mode;

        // Drive config methods
        pros::Task *tank_task;
        void tank();
        pros::Task *hDrive_task;
        void hdrive();
        pros::Task *holonomic_task;
        void holonomic_drive();
        pros::Task *custom_driveConf_task;
        void custom_config();
        // pros::Task *xdrive_task;
        // void xdrive();

        // Drive mode methods
        void updateAxis();
        ctrler_axis_s tank_drive();
        ctrler_axis_s single_stick_arcade_right();
        ctrler_axis_s single_stick_arcade_left();
        ctrler_axis_s split_arcade_right();
        ctrler_axis_s split_arcade_left();
        ctrler_axis_s custom_mode();

        // All scaling factors and other variables
        double square_scale(double input);
        double sin_scale(double input);
        double normalize_joystick(double input);

        // Movement methods
        // void setMotors();

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
        driveClass* init();
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



/*
Use different music for clips at purdue???
Music Stops abruptly
Angled focus, someone walks into twlight
Places both the purdue awards
Gets email of worlds qual
Cuts to both robots being chucked out the window
and doinker
Sits down at computer
Opens fusion 360
Creates new cad files
~1 eternity later
grin?
~clips to pressing enter on keyboard
~some transition
Start building montage


Start with cornfield fails
Wisco failure
Zoom out from screen watching video of clips
we all leave sad
last person gets an email from vex about worlds qual
sits down at computer
opens fusion 360
creates new cad files
~1 eternity later
grin?
~clips to pressing enter on keyboard
~some transition
Start building montage
*/