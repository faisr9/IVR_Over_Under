#include "pookie/controls.h"

// Will add driver profiles later
void controls() {
    // for debugging
	pros::Task odom_task{[=] {
		while (1) {
			tank_drive_15.getOdom().updatePosition();
			pros::delay(50);
		}
	}};

    pros::lcd::set_text(1, "Running Controls");

    while(1) {
        //ACTIVATE DRIVE
        if(pros::competition::is_connected())
            tank_drive_15.change_drive_mode(0);
        else
        {
            if(gui::tank_drive)
                tank_drive_15.change_drive_mode(1);
            else
                tank_drive_15.change_drive_mode(0);
        }
        
        tank_drive_15.toggle_drive_mode();

        //INTAKE CONTROLS
        if (ctrl_master.get_digital(BUTTON_R1)){
            Intake::getInstance()->set_power(-12000);
        }
        else if (ctrl_master.get_digital(BUTTON_R2)){
            Intake::getInstance()->set_power(12000);
        } else {
            Intake::getInstance()->set_power(0);
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
