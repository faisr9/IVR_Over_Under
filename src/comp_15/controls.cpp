#include "comp_15/controls.h"

void controls() {
    pros::lcd::set_text(1, "driving");
    while(1) {
        drive.toggle_drive_mode();

        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_A)){
            Intake::getInstance()->set_power(12000);
        }
        else if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)){
            Intake::getInstance()->set_power(-12000);
        } else {
            Intake::getInstance()->set_power(0);
        }

        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_A)){
            catapult_instance->cycle();
        }
        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_X)){
            catapult_instance->prime();
        }
        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)){
            catapult_instance->release();
        }

        pros::delay(15);
    }
}