#include "comp_15/controls.h"
// #include "comp_15/include_list.h"
#include "common_code/PID.h"
#include "common_code/traditional_drive.h"
#include "comp_15/devices.h"

// Will add driver profiles later
void controls() {
    pros::lcd::set_text(1, "Running Controls");

    drive.getOdom().initTracker(0, 0, 0);
    pros::delay(50);


    // DoinkerClass::doinker_move doinker_state = DoinkerClass::UP;
    PID PID_lateral = PID(.6,0,.3);
    PID PID_turn = PID(.6,0,.3);
    double pid_turn_out;
    double pid_lat_out;
    double y;
    double target;
    double theta;
    double tolerance = 1;
    int count = 0;

    while(1) {
        //ACTIVATE DRIVE
        // if(pros::competition::is_connected())
        //     drive.change_drive_mode(0);
        // else
        // {
        //     if(gui::tank_drive)
        //         drive.change_drive_mode(1);
        //     else
        //         drive.change_drive_mode(0);
        // }
        
        // drive.toggle_drive_mode();

        //***************** PID TESTING *****************
        // if (ctrl_master.get_digital(BUTTON_X)){
                drive.getOdom().updatePosition();
                y=drive.getOdom().getY()*39.37;
                theta = drive.get_imu().get_heading();
                switch (count){
                    case 0:
                        target = 24;
                        theta = 0;
                        if(PID_lateral.getState().targetReached){
                            count++;
                            ctrl_master.rumble(".");
                            PID_turn.resetPID();
                            PID_lateral.resetPID();
                            pros::delay(150);
                        }
                        break;
                    // case 1:
                    //     target = 0;
                    //     if(PID_tk.getState().targetReached){
                    //         count++;
                    //         ctrl_master.rumble(".");
                    //         PID_tk.resetPID();
                    //     }
                        // target = 0;
                        // if(PID_tk.getState().targetReached){
                        //     count++;
                        //     ctrl_master.rumble("..");
                        //     PID_tk.resetPID();
                        //     pros::delay(150);
                        // }
                        // break;
                }

                pid_lat_out = PID_lateral.updatePID(target, y, tolerance); //1m=39.37in
                pid_turn_out = PID_turn.updatePID(target, theta, tolerance);
                drive.tank_with_power(pid_lat_out,pid_turn_out);

                lcd::print(1, "PID: %.2f, %.2f", pid_lat_out, pid_turn_out);
                lcd::print(2, "Y: %.2f", y);
                lcd::print(3, "Target Reached: %.2f, %.2f", PID_lateral.getState().targetReached, PID_turn.getState().targetReached);
                lcd::print(4, "Error: %.2f, %.2f", PID_lateral.getState().error, PID_turn.getState().error);

                pros::delay(15);
        // }
        //***********************************************

        // // // TESTING CODE, REMOVE THIS BEFORE MERGING TO MASTER
        // // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
        // //     turnToAngle(drive, 0, 5.0, false, 1.9, wait_time);
        // // }

        // //INTAKE CONTROLS
        // if (ctrl_master.get_digital(BUTTON_R2)){
        //     Intake::getInstance()->set_power(12000);
        // }
        // else if (ctrl_master.get_digital(BUTTON_R1)){
        //     Intake::getInstance()->set_power(-12000);
        // } else {
        //     Intake::getInstance()->set_power(0);
        // }

        // // CATAPULT CONTROLS
        // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
        //     if (cata_limit.get_value()) {
        //         CompetitionCatapult::getInstance()->set_cata_mode("RP");
        //     } else {
        //         CompetitionCatapult::getInstance()->set_cata_mode("P");
        //     }
        // } else if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_L2)) {
        //     CompetitionCatapult::getInstance()->set_cata_mode("I");
        // }

        // //PNEUMATICS CONTROLS
        // if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)) {
        //     Pneumatics::getInstance()->getWings()->toggle();
        // }
        // else if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
        //     Pneumatics::getInstance()->getFloorBrake()->toggle();
        // }

        // // pros::lcd::set_text(6, "Doinker pot: " + std::to_string(doinker_pot.get_value()));
        // // pros::lcd::set_text(7, "Doinker desired state: " + std::to_string(doinker_state));

        // if (!pros::competition::is_connected())
        // {
        //     if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
        //         doinker_state = DoinkerClass::UP;
        //     } else if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
        //         doinker_state = DoinkerClass::DOWN;
        //     } 
        // }
        // DoinkerClass::getInstance()->move(doinker_state);

        // // 3600 to 1240
        
        // // Keeps doinkerUp during driver control
        pros::delay(15);
    }
}