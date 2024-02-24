#include "comp_18/controls.h"

// Will add driver profiles later
void controls() {
    pros::lcd::set_text(1, "Running Controls");
    while(1) {
        //ACTIVATE DRIVE
        if(pros::competition::is_connected())
            tank_drive_18.change_drive_mode(0);
        else
        {
            if(gui::tank_drive)
                tank_drive_18.change_drive_mode(1);
            else
                tank_drive_18.change_drive_mode(0);
        }
        
        tank_drive_18.toggle_drive_mode();

        //INTAKE CONTROLS
        if (ctrl_master.get_digital(BUTTON_L2)){
            Intake::getInstance()->set_power(12000);
        }
        else if (ctrl_master.get_digital(BUTTON_R2)){
            Intake::getInstance()->set_power(-12000);
        } else {
            Intake::getInstance()->set_power(0);
        }

        //PNEUMATICS CONTROLS
        if(ctrl_master.get_digital_new_press(BUTTON_L1)) {
            Pneumatics::getInstance()->toggleLeft();
        }
        if(ctrl_master.get_digital_new_press(BUTTON_R1)) {
            Pneumatics::getInstance()->toggleRight();
        }
        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
            Pneumatics::getInstance()->getClimber()->toggle();
        }

        pros::delay(15);
    }
}
