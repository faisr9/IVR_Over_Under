#include "comp_15/controls.h"

// Will add driver profiles later
void controls() {
    pros::lcd::set_text(1, "Running Controls");

    DoinkerClass::doinker_move doinker_state = DoinkerClass::UP;

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

        pros::lcd::set_text(6, "Doinker pot: " + std::to_string(doinker_pot.get_value()));
        pros::lcd::set_text(7, "Doinker desired state: " + std::to_string(doinker_state));

    
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)) {
            doinker_state = DoinkerClass::UP;
        } else if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) {
            doinker_state = DoinkerClass::DOWN;
        } 
        DoinkerClass::getInstance()->move(doinker_state);

        // 3600 to 1240

        // Keeps doinkerUp during driver control
        pros::delay(15);
    }
}