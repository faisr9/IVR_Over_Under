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

        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_R2)){
            CompetitionCatapult::getInstance()->cycle();
        }
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)){
            CompetitionCatapult::getInstance()->prime();
        }
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_R1)){
            CompetitionCatapult::getInstance()->release();
        }

        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) {
            cata.move_voltage(12000);
        } else {
            cata.move_voltage(0);
        }

        //PNEUMATICS CONTROLS
        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
            Pneumatics::getInstance()->getWings()->toggle();
        }

        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
            Pneumatics::getInstance()->getFloorBrake()->toggle();
        }

        pros::lcd::set_text(3, "Left: " + std::to_string(cata_left.get_voltage()));
        pros::lcd::set_text(3, "Right: " + std::to_string(cata_left.get_voltage()));

        pros::delay(15);
    }
}