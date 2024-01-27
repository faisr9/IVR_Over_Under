#include "comp_15/controls.h"

void controls() {
    while(1) {
        drive.toggle_drive_mode();

        pros::lcd::set_text(1, "driving");

        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_A)){
            IntakeClass::getInstance()->set_power(12000);
        }
        else if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)){
            IntakeClass::getInstance()->set_power(-12000);
        }

        pros::delay(20);
    }
}