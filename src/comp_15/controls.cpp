#include "comp_15/controls.h"

int cnt = 0;

void controls() {
    pros::lcd::set_text(1, "Running Controls");
    while(1) {
        //ACTIVATE DRIVE
        drive.toggle_drive_mode();

        //INTAKE CONTROLS
        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_A)){
            Intake::getInstance()->set_power(12000);
        }
        else if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)){
            Intake::getInstance()->set_power(-12000);
        } else {
            Intake::getInstance()->set_power(0);
        }

        //CATAPULT CONTROLS
        // if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_A)){
        //     catapult_instance->cycle();
        // }
        // if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_X)){
        //     catapult_instance->prime();
        // }
        // if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)){
        //     catapult_instance->release();
        // }

        //PNEUMATICS CONTROLS
        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
            Pneumatics::getInstance()->getWings()->toggle();
            // std::string status = to_string(Pneumatics::getInstance()->getWings()->getStatus() ? 1 : 0);
            // pros::lcd::set_text(3, status);
        }

        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
            Pneumatics::getInstance()->getKickstand()->toggle();
        }


        pros::delay(15);
    }
}