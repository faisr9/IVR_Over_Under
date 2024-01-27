#include "comp_15/comp15_includeList.h"

void controls() {
    while(1) {
        drive.toggle_drive_mode();
        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_A)){
            IntakeClass::getInstance()->set_power(12000);
        }
        else if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)){
            IntakeClass::getInstance()->set_power(-12000);
        }
    }
}