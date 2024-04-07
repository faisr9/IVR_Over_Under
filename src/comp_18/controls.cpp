#include "comp_18/controls.h"

// Will add driver profiles later
void controls() {


    // for debugging
	pros::Task odom_task{[=] {
		while (1) {
			tank_drive_18.getOdom().updatePosition();
			pros::delay(50);
		}
	}};


    pros::lcd::set_text(1, "Running Controls");

    //climbing vars
    int count=0;
    double oldAvg = 0;
    double avg = 0;
    bool climbFlagUp= 0;
    bool climbFlagDown= 0;
    int angle;

    while(1) {
        if(Pneumatics::getInstance()->getPTO()->getStatus()){
            tank_drive_18.get_motor_group(0).set_brake_modes(pros::motor_brake_mode_e_t::E_MOTOR_BRAKE_HOLD);
            tank_drive_18.get_motor_group(1).set_brake_modes(pros::motor_brake_mode_e_t::E_MOTOR_BRAKE_HOLD);
        } else {
            tank_drive_18.get_motor_group(0).set_brake_modes(pros::motor_brake_mode_e_t::E_MOTOR_BRAKE_COAST);
            tank_drive_18.get_motor_group(1).set_brake_modes(pros::motor_brake_mode_e_t::E_MOTOR_BRAKE_COAST);
        }

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
        if (ctrl_master.get_digital(BUTTON_R2)){
            Intake::getInstance()->set_power(-12000);
        }
        else if (ctrl_master.get_digital(BUTTON_R1)){
            Intake::getInstance()->set_power(12000);
        } else {
            Intake::getInstance()->set_power(0);
        }

        //PNEUMATICS CONTROLS
        if(ctrl_master.get_digital_new_press(BUTTON_L2)) {
            Pneumatics::getInstance()->toggleWings();
        }
        // if(ctrl_master.get_digital_new_press(BUTTON_R1)) {
        //     Pneumatics::getInstance()->toggleRight();
        // }
        if(ctrl_master.get_digital_new_press(BUTTON_L1)) {
            Pneumatics::getInstance()->getIntake()->toggle();
        }   

        //Climbing
        while(ctrl_master.get_digital_new_press(BUTTON_UP) || climbFlagUp) {
            climbFlagUp = 1;
            angle = 50;
            Pneumatics::getInstance()->getPTO()->on();
            pros::delay(50);
            avg = (tank_drive_18.get_motor_group(0).get_positions()[0]+tank_drive_18.get_motor_group(1).get_positions()[0]
                    +tank_drive_18.get_motor_group(0).get_positions()[1]+tank_drive_18.get_motor_group(1).get_positions()[1]
                    +tank_drive_18.get_motor_group(0).get_positions()[2]+tank_drive_18.get_motor_group(1).get_positions()[2]
                    +tank_drive_18.get_motor_group(0).get_positions()[3]+tank_drive_18.get_motor_group(1).get_positions()[3])/8;
            if(!count){ oldAvg = avg; count++; }
            if(std::abs(avg-oldAvg)*(3/1)<angle)
                tank_drive_18.move_with_power(0.5);
            else{
                tank_drive_18.move_with_power(0);
            }
            oldAvg = avg;
        }

        //GET DOWN MDM. PREZ
        while(ctrl_master.get_digital_new_press(BUTTON_DOWN) || climbFlagDown) {
            climbFlagDown = 1;
            angle = 50;
            pros::delay(50);
            avg = (tank_drive_18.get_motor_group(0).get_positions()[0]+tank_drive_18.get_motor_group(1).get_positions()[0]
                    +tank_drive_18.get_motor_group(0).get_positions()[1]+tank_drive_18.get_motor_group(1).get_positions()[1]
                    +tank_drive_18.get_motor_group(0).get_positions()[2]+tank_drive_18.get_motor_group(1).get_positions()[2]
                    +tank_drive_18.get_motor_group(0).get_positions()[3]+tank_drive_18.get_motor_group(1).get_positions()[3])/8;
            if(!count){ oldAvg = avg; count++; }
            if(std::abs(avg-oldAvg)*(3/1)<angle)
                tank_drive_18.move_with_power(-0.3);
            else{
                tank_drive_18.move_with_power(0);
                Pneumatics::getInstance()->getPTO()->off();
            }
            oldAvg = avg;
        }
        
        pros::delay(15);
    }
}
