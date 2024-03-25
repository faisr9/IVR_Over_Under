#include "common_code/drive_builder.h"

drive_builder::drive_builder(drive_config_e config, pros::Controller *controller) {
    drive_config = config;
    drive_controller = controller;
}

drive_builder &drive_builder::with_motors(pros::Motor_Group *left, pros::Motor_Group *right) {
    
    left_drive = left;
    right_drive = right;
}

drive_builder &drive_builder::with_motors(motor *left_1, motor *left_2, motor *right_1, motor *right_2) {

}

drive_builder &drive_builder::with_motors(motor *left_1, motor *left_2, motor *left_3, motor *right_1, motor *right_2, motor *right_3) {

}

drive_builder &drive_builder::with_motors(motor *left_1, motor *left_2, motor *left_3, motor *left_4, motor *right_1, motor *right_2, motor *right_3, motor *right_4) {

}

drive_builder &drive_builder::add_strafe_motor(motor *strafe) {

}

drive_builder &drive_builder::set_default_drive_mode(drive_mode_e mode) {

}

drive_builder &drive_builder::init() {

}