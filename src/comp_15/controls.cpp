#include "comp_15/controls.h"
// #include "comp_15/include_list.h"

// Will add driver profiles later
void controls() {
    pros::lcd::set_text(1, "Running Controls");

    double p = 1.9;

    while(1) {
        //ACTIVATE DRIVE
        if(pros::competition::is_connected())
            drive.change_drive_mode(0);
        else
        {
            if(gui::tank_drive)
                drive.change_drive_mode(1);
            else
                drive.change_drive_mode(0);
        }
        
        drive.toggle_drive_mode();

        // // TESTING CODE, REMOVE THIS BEFORE MERGING TO MASTER
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
            turnToAngle(drive, 0, 2.5, false, p);
        }

        //INTAKE CONTROLS
        if (ctrl_master.get_digital(BUTTON_R2)){
            Intake::getInstance()->set_power(12000);
            p += 0.1;
        }
        else if (ctrl_master.get_digital(BUTTON_R1)){
            Intake::getInstance()->set_power(-12000);
            p -= 0.1;
        } else {
            Intake::getInstance()->set_power(0);
        }


        //PNEUMATICS CONTROLS
        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)) {
            Pneumatics::getInstance()->getWings()->toggle();
        }
        else if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
            Pneumatics::getInstance()->getFloorBrake()->toggle();
        }

        // pros::lcd::set_text(6, "Doinker pot: " + std::to_string(doinker_pot.get_value()));
        // pros::lcd::set_text(7, "Doinker desired state: " + std::to_string(doinker_state));

        // 3600 to 1240
        
        // Keeps doinkerUp during driver control

        pros::lcd::set_text(2, std::to_string(p));

        pros::delay(15);
    }
}