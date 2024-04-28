#include "dookie/controls.h"

void controls() {
    pros::lcd::set_text(1, "Running Controls");

    while(1) {
        // ACTIVATE DRIVE
        tank_drive_18.change_drive_mode(1);
        
        tank_drive_18.toggle_drive_mode();

        //INTAKE CONTROLS
        if (ctrl_master.get_digital(BUTTON_R1)){
            Intake::getInstance()->toggle_on();
        }
        else if (ctrl_master.get_digital(BUTTON_R2)){
            Intake::getInstance()->toggle_reverse();
        } else {
            Intake::getInstance()->stop();
        }

        //PNEUMATICS CONTROLS
        if(ctrl_master.get_digital_new_press(BUTTON_L2)) {
            Pneumatics::getInstance()->getWings()->toggle();
        }

        if(ctrl_master.get_digital_new_press(BUTTON_L1)) {
            Pneumatics::getInstance()->getIntake()->toggle();
        }   

        if(ctrl_master.get_digital_new_press(BUTTON_UP)) {
            Pneumatics::getInstance()->getTopHang()->toggle();
            pros::delay(25);
        }

        if(ctrl_master.get_digital_new_press(BUTTON_DOWN)) {
            Pneumatics::getInstance()->getSideHang()->toggle();
            pros::delay(25);
        }
        
        pros::delay(15);
    }
}
