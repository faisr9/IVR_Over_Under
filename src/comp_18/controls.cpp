#include "comp_18/controls.h"

int cnt = 0;

void controls() {
    pros::lcd::set_text(1, "Running Controls");
    while(1) {
        //ACTIVATE DRIVE
        tank_drive_18.toggle_drive_mode();

        // //INTAKE CONTROLS
        // if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_A)){
        //     Intake::getInstance()->set_power(12000);
        // }
        // else if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)){
        //     Intake::getInstance()->set_power(-12000);
        // } else {
        //     Intake::getInstance()->set_power(0);
        // }

        // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_R2)){
        //     CompetitionCatapult::getInstance()->cycle();
        // }
        // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)){
        //     CompetitionCatapult::getInstance()->prime();
        // }
        // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_R1)){
        //     CompetitionCatapult::getInstance()->release();
        // }

        // if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) {
        //     cata.move_voltage(12000);
        // } else {
        //     cata.move_voltage(0);
        // }

        //PNEUMATICS CONTROLS
        // this is all test code, we need to test if the toggle, on, and off commands work for a piston.
        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
            Pneumatics::getInstance()->getWingLeft()->toggle();
        }

        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
            Pneumatics::getInstance()->getWingLeft()->on();
        }

        if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
            Pneumatics::getInstance()->getWingLeft()->off();
        }

        // pros::lcd::set_text(3, "Left: " + std::to_string(cata_left.get_voltage()));
        // pros::lcd::set_text(3, "Right: " + std::to_string(cata_left.get_voltage()));

        pros::delay(15);
    }
}