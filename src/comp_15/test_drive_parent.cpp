#include "common_code/drive_parent.h"


void my_funct() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);

    while(1) {

        double normalized = DriveParent::normalize_joystick(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));

        double square_scale = DriveParent::square_scale(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));

        pros::lcd::set_text(1, "Norm joystick: " + std::to_string(normalized));
        pros::lcd::set_text(2, "Square scale: " + std::to_string(square_scale));


        pros::delay(50);
    }
}