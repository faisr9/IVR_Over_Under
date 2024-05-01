#include "pookie/auton.h"

void ctrlerHold() {
    while(1) {
        if (ctrl_master.get_digital(BUTTON_X)) {
            break;
        }
        pros::delay(20);
    }
}

// Auton Methods 
void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor, double lookAhead, double turnP){
	for (auto& vec : moveVec) {
		for (auto& val : vec) { val = convert::inToM(val); }
	}
	followPath(moveVec, tank_drive_15, angle, isReversed, isSpinAtEnd, false, lookAhead, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, turnP);
}

// BEGIN AUTON METHODS //

const double AUTON_DURATION = 45000 - 500; // 45 seconds minus 500 milliseconds for safety of threads exiting
void main_auton(bool wp) {

    const double STARTING_X = convert::inToM(13);
    const double STARTING_Y = convert::inToM(22.5);
    const double STARTING_ANGLE = 135;
    const double AUTON_START_TIME = pros::millis();
    double AUTON_RUN_TIME = 0;
    bool end_auton = false;

    tank_drive_15.getOdom().initTracker(STARTING_X, STARTING_Y, STARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[&] {
        while (!end_auton) {
            tank_drive_15.getOdom().updatePosition();
            AUTON_RUN_TIME = pros::millis() - AUTON_START_TIME;
            if (AUTON_RUN_TIME >= AUTON_DURATION) {
                end_auton = true;
            }

            pros::delay(20);
        }
    }};

    // Auton code here
    pros::Task auton_task {[=] {
        if (wp) {
            // Get Alliance Triball
            Intake::getInstance()->toggle_on(600);
            delay(600);
            Intake::getInstance()->stop();
            Pneumatics::getInstance()->getWings()->on();
            delay(150);
            movePID(tank_drive_15, 9, 135,1200);

            turnToAngle(tank_drive_15, 120, 2);
            Pneumatics::getInstance()->getWings()->off();
            Intake::getInstance()->toggle_on(-600);
            delay(100);
            movePID(tank_drive_15, 11, 150, 1300);

            turnToAngle(tank_drive_15, 110, 2);
            // vector<vector<double>> toOtherDepot = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getX())}, 
            //                                         {2.5*24, 7.5}, {71, 7.5}, {108, 7.5}}; //pickup 1st WP triball along path 
        } else {
            int bowls = 4;

            Intake::getInstance()->toggle_on(600);
            delay(600);
            Intake::getInstance()->stop();
            turnToAngle(tank_drive_15, 315, 1, false, 2.1);
            movePID(tank_drive_15, -11, 315, 1200);
            Pneumatics::getInstance()->getWings()->on();

            // Oscillate to bowl 
            for (int i=0; i<bowls; i++) {
                delay(325);
                turnToAngle(tank_drive_15, 265, 5, false, 2.75);
                delay(230);
                turnToAngle(tank_drive_15, 315, 5, false, 2.1);
            }

            tank_drive_15.move_with_power((5000.0/12000.0) * 127);
            delay(300);
            tank_drive_15.brake();
            turnToAngle(tank_drive_15, 135, 7, false, 2.1);

            pros::Task wing_void_task {[=] {
                delay(1400);
                Pneumatics::getInstance()->getWings()->off();
                delay(1000);
                Pneumatics::getInstance()->getWings()->on();
            }};
        }

        vector<vector<double>> toOtherDepot = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())}, 
                                                {2*23.5, 6}, {4.5*23.5, 6.5}, {113, 8.5}}; //pickup 1st WP triball along path 
        move(toOtherDepot, 50, false, true, 1);
        Pneumatics::getInstance()->getWings()->on();
        turnToAngle(tank_drive_15, 50, 2);
        ////////
        // Score WP Triballs
        if(wp) {
            vector<vector<double>> toGoal = {toOtherDepot.back(), 
                                                {116, 12}, {125,16}, {126, 22}}; //pickup 2nd WP triball along path
            move(toGoal, 350, false, false, 2,.5);

            turnToAngle(tank_drive_15, 10, 2);
            Intake::getInstance()->toggle_on(-600);
            Pneumatics::getInstance()->getWings()->off();
            delay(200);
            turnToAngle(tank_drive_15, 220, 2);
            movePID(tank_drive_15, -9, 220, 1000);
            turnToAngle(tank_drive_15, 185, 2);
            movePID(tank_drive_15, 4, 220, 1000);
            Intake::getInstance()->stop();
        } else {
            Intake::getInstance()->toggle_on(); //rev
            turnToAngle(tank_drive_15, 45, 2);
            movePID(tank_drive_15, 17, 45, 1700);
            movePID(tank_drive_15, -9, 45, 1700);
            turnToAngle(tank_drive_15, 220, 2);
        }
        double heading_x = tank_drive_15.getOdom().getHeading();
        Pneumatics::getInstance()->getWings()->off();
        for(int i=0; i<(wp?1:2); i++){
            delay(50);
            tank_drive_15.move_with_power(-100);
            // waitUntil(tank_drive_15.getOdom().getY() >= convert::inToM(25.5));
            delay(450);
            tank_drive_15.move_with_power(0);
            movePID(tank_drive_15, 11, 185,1200);
            turnToAngle(tank_drive_15, heading_x, 6);
        }
        // Pneumatics::getInstance()->getIntake()->off();
        ///////

        // Sweep Dookie Sauce

        movePID(tank_drive_15, 4, 180, 500);
        // Intake::getInstance()->toggle_on(-150);
        turnToAngle(tank_drive_15, 250, 2);
        delay(50);
        pros::Task wings_sweep_delay {[=]{ delay(1500); Pneumatics::getInstance()->getWings()->on(); }};
        Intake::getInstance()->set_power(-12000);
        // vector<vector<double>> sweep = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())},
        //                                     {100,33}, {84.5, 40}, {84, 61}, {91, 64}, {96, 64}, {115, 64}}; //pickup 2nd WP triball along path
        vector<vector<double>> sweep1 = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())},
                                            {100, 40}, {92, 40}}; //pickup 2nd WP triball along path
        move(sweep1, 270, false, false, 2, .35);
        turnToAngle(tank_drive_15, 0, 2);
        vector<vector<double>> sweep2 = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())},
                                            {86, 40}, {86, 59}, {91, 60}, {96, 60}, {115, 60}}; //pickup 2nd WP triball along path
        move(sweep2, 90, false, false, 2, .35);
        Intake::getInstance()->toggle_on();
        turnToAngle(tank_drive_15, 90, 2);
        delay(50);
        // ram in front of goal
        for(int i=0; i<2; i++){
            tank_drive_15.move_with_power(90);
            delay(500);
            tank_drive_15.move_with_power(0);
            movePID(tank_drive_15, -14, 90, 1200);  
        }
        tank_drive_15.move_with_power(-90);
        delay(500);
        tank_drive_15.brake();
        // movePID(tank_drive_15, -14, 90, 1200);  
        turnToAngle(tank_drive_15, 0, 5);
        /////
        // if(wp) {
        //     // TO CLIMB
        //     turnToAngle(tank_drive_15, 210, 3);
        //     Pneumatics::getInstance()->getIntake()->on();
        //     Intake::getInstance()->toggle_on(600);
        //     // vector<vector<double>> toClimb = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())}, 
        //     //                                     {86.5, 27.5}};
        //     //vector<vector<double>> toClimb = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())}, 
        //     //                                    {87, 24}};
        //     // move(toClimb, 180, false, false, 1.65, .35);
        //     //move(toClimb, 270, false, false, 1.65, .35);
        //     movePID(tank_drive_15, 45, 210, 900);
        //     turnToAngle(tank_drive_15, 270, 2);
        //     movePID(tank_drive_15, 6, 270, 250);

            

        //     tank_drive_15.split_tank_with_power(20, 0);
        //     delay(250);
        //     // tank_drive_15.split_tank_with_power(10,0);
        // }
    }};
    // auton_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);
	
    // End of auton
    int cnt1 = 0;
    int cnt2 = 0;
    // int cnt3 = 0;
    while(!end_auton) {
        if (!wp) {
            if(tank_drive_15.getOdom().getX() > convert::inToM(1.8*24)&& !cnt1){
                cnt1++;
                Intake::getInstance()->stop();
            }
            if(tank_drive_15.getOdom().getX() >= convert::inToM(100) && tank_drive_15.getY() >= convert::inToM(10) && !cnt2){
                Pneumatics::getInstance()->getWings()->on();
                cnt2++;
            }
            // if(tank_drive_15.getOdom().getX() >= convert::inToM(115) && tank_drive_15.getY() >= convert::inToM(19) && !cnt3){
            //     Pneumatics::getInstance()->getWings()->off();
            //     Intake::getInstance()->toggle_on();
            //     cnt3++;
            // }
        }
        pros::delay(20);
    }
    
    if (odom_task.get_state() == E_TASK_STATE_RUNNING || odom_task.get_state() == E_TASK_STATE_READY || odom_task.get_state() == E_TASK_STATE_SUSPENDED) {
        odom_task.remove(); // Delete the odom task
    }
    if (auton_task.get_state() == E_TASK_STATE_RUNNING || auton_task.get_state() == E_TASK_STATE_READY){
        auton_task.suspend(); // Delete the auton task
        auton_task.remove();
    }
    // auton_task.remove(); // Delete the auton task
    // Stop everything
    // Drive Stop
    tank_drive_15.move_with_power(0);
    // Intake Stop
    Intake::getInstance()->stop();
    // Wings Close
    // Pneumatics::getInstance()->getWings()->off();
}

// void non_win_point_auton() {

//     const double STARTING_X = convert::inToM(13);
//     const double STARTING_Y = convert::inToM(22.5);
//     const double STARTING_ANGLE = 135.0;
//     const double AUTON_START_TIME = pros::millis();
//     double AUTON_RUN_TIME = 0;
//     bool end_auton = false;

//     tank_drive_15.getOdom().initTracker(STARTING_X, STARTING_Y, STARTING_ANGLE);
//     pros::delay(50);

//     pros::Task odom_task{[&] {
//         while (!end_auton) {
//             tank_drive_15.getOdom().updatePosition();
//             AUTON_RUN_TIME = pros::millis() - AUTON_START_TIME;
//             if (AUTON_RUN_TIME >= AUTON_DURATION) {
//                 end_auton = true;
//             }
//             pros::delay(20);
//         }
//     }};

//     // Auton code here
//     pros::Task auton_task {[=] {
		

//         vector<vector<double>> toOtherDepot = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())}, 
//                                                 {2*23.5, 9}, {4.5*23.5, 6.5}, {113, 11.5}}; //pickup 1st WP triball along path 
//         move(toOtherDepot, 50, false, true, 1);

//         movePID(tank_drive_15, 14, 50, 2000);
//         turnToAngle(tank_drive_15, 0, 2);
//         movePID(tank_drive_15, 9, 15, 1500, 1.2, 1);
//         Intake::getInstance()->toggle_on(600);
//         movePID(tank_drive_15, -9, 15, 1500, 1.2, 1);
//         Pneumatics::getInstance()->getWings()->off();
//         turnToAngle(tank_drive_15, 210, 2, false, 1.74);
//         Intake::getInstance()->stop();

//         // Ramp to goal
//         for(int i=0; i<3; i++) {
//             delay(50);
//             tank_drive_15.move_with_power(-100);
//             delay(450);
//             tank_drive_15.move_with_power(0);
//             movePID(tank_drive_15, 11, 185,1200);
//         }

//         // vector<vector<double>> toGoal = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())},
//         //                                     {24, 19}, {34, 15}, {45, 10}, {65, 8}, {100, 8}, {110, 19}, {120, 27}}; //pickup 2nd WP triball along path

//         // Pneumatics::getInstance()->getWings()->off();
//         // Intake::getInstance()->toggle_on(-600);
//         // delay(100);
//         // movePID(tank_drive_15, 11, 150, 1300);

//         // tank_drive_15.move_with_power((-5000.0/12000.0) * 127); //tune this value for new postion
//         // turnToAngle(tank_drive_15, 315, 1, false, 2.1);
//         // // Begin Match loading
//         // Pneumatics::getInstance()->getWings()->on();
//         // delay(450);
//         // tank_drive_15.brake();

//         // // Oscillate to bowl 
//         // for (int i=0; i<bowls; i++) {
//         //     delay(325);
//         //     turnToAngle(tank_drive_15, 265, 5, false, 2.75);
//         //     delay(230);
//         //     turnToAngle(tank_drive_15, 315, 5, false, 2.1);
//         // }

//         // // Push triballs away from robot
//         // tank_drive_15.move_with_power((5000.0/12000.0) * 127);
//         // delay(300);
//         // tank_drive_15.brake();
//         // turnToAngle(tank_drive_15, 135, 7, false, 2.1);
//         // Intake::getInstance()->toggle_on(600);
//     ///////////////////////////////////////////////////////////////////////////////////////////
//         /** POSITION: X:~24.4 Y:~19 H:~135.8 */
//         // Bowl to other goal
        
//         /*
//         vector<vector<double>> bowl_path = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())},
//                                                 {34, 15}, {45, 10}, {65, 8}, {100, 8}, {110, 19}, {120, 27}};
//         move(bowl_path, 50, false, false, 0.89, 0.43, 1.44, 45);
//         movePID(tank_drive_15, -9, 50, 1000);
//         turnToAngle(tank_drive_15, 230, 2);

//         vector<vector<double>> charging_up = {{convert::mToIn(tank_drive_18.getOdom().getX()), convert::mToIn(tank_drive_18.getOdom().getY())},
//                                                 {120, 27}};
//         for(int i=0; i<3; i++){
//             tank_drive_15.move_with_power(-90);
//             delay(400);
//             tank_drive_15.move_with_power(0);
//             move(charging_up, 230, false, false, 0.89, 0.43, 1.44, 45);
//         }
//         */

//     }};
//     auton_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);

//     // End of auton
//     while(!end_auton) {
//         pros::delay(20);
//     }

//     if (odom_task.get_state() == E_TASK_STATE_RUNNING || odom_task.get_state() == E_TASK_STATE_READY || odom_task.get_state() == E_TASK_STATE_SUSPENDED) {
//         odom_task.remove(); // Delete the odom task
//     }
//     if (auton_task.get_state() == E_TASK_STATE_RUNNING || auton_task.get_state() == E_TASK_STATE_READY){
//         auton_task.suspend(); // Delete the auton task
//         auton_task.remove();
//     }
//     // auton_task.remove(); // Delete the auton task
//     // Stop everything
//     // Drive Stop
//     tank_drive_15.move_with_power(0);
//     // Intake Stop
//     Intake::getInstance()->stop();
//     // Wings Close
//     // Pneumatics::getInstance()->getWings()->off();
// }