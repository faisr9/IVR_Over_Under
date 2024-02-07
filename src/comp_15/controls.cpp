#include "comp_15/controls.h"

int cnt = 0;

// Will add driver profiles later
void controls() {
    pros::lcd::set_text(1, "Running Controls");
    while(1) {
        //ACTIVATE DRIVE
        drive.toggle_drive_mode();

        //INTAKE CONTROLS
        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_A) || ctrl_master.get_digital(BUTTON_R1)){
            Intake::getInstance()->set_power(12000);
        }
        else if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B) || ctrl_master.get_digital(BUTTON_R2)){
            Intake::getInstance()->set_power(-12000);
        } else {
            Intake::getInstance()->set_power(0);
        }

        // CATAPULT CONTROLS
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
            if (cata_limit.get_value()) {
                CompetitionCatapult::getInstance()->set_cata_mode("RP");
            } else {
                CompetitionCatapult::getInstance()->set_cata_mode("P");
            }
        } else if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_LEFT)) {
            CompetitionCatapult::getInstance()->set_cata_mode("I");
        }

        //PNEUMATICS CONTROLS
        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
            Pneumatics::getInstance()->getWings()->toggle();
        }
        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
            Pneumatics::getInstance()->getFloorBrake()->toggle();
        }

        pros::delay(15);
    }
}