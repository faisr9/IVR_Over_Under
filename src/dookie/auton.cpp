#include "dookie/auton.h"

void ctrlerHold() {
    while(1) {
        if (ctrl_master.get_digital(BUTTON_X)) {
            break;
        }
        pros::delay(20);
    }
}

void move(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor,  double lookAhead, double turnP, double turnTol){
	for (auto& vec : moveVec) {
		for (auto& val : vec) { val = convert::inToM(val); }
	}
	followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, lookAhead, turnTol, 200.0 / speedfactor, 600.0 / speedfactor, 40.0 / speedfactor, false, turnP);
}

// BEGIN AUTON METHODS //
const double AUTON_DURATION = 45000 - 500; // 45 seconds minus 500 milliseconds for safety of threads exiting

void win_point_auton() {

    const double STARTING_X = convert::inToM(38.75);
    const double STARTING_Y = convert::inToM(16.5);
    vector<double> STARTING_POS = {STARTING_X, STARTING_Y};
    const double STARTING_ANGLE = 0.0;
    const double AUTON_START_TIME = pros::millis();
    double AUTON_RUN_TIME = 0;
    bool end_auton = false;

    tank_drive_18.getOdom().initTracker(STARTING_X, STARTING_Y, STARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[&] {
        while (!end_auton) {
            tank_drive_18.getOdom().updatePosition();
            AUTON_RUN_TIME = pros::millis() - AUTON_START_TIME;
            if (AUTON_RUN_TIME >= AUTON_DURATION) {
                end_auton = true;
            }

            pros::delay(20);
        }
    }};

    // Auton code here
    pros::Task auton_task {[=] {
        int bowls = 10;

        // Drop intake
        Intake::getInstance()->toggle_on();
        // Pneumatics::getInstance()->getWings()->on();
        // delay(200); // Robot settle
        // Pneumatics::getInstance()->getWings()->off();
///////////////////////////////////////////////////////////////////////////////////////////

        // Move to get first ball
        vector<vector<double>> path_triball_1 = {STARTING_POS, {57, 37.23}, {46.1, 52.65}};
        move(path_triball_1, 339, false, false, 0.69, 0.5, 1.12, 45);

        // Back up to push first ball
        vector<vector<double>> path_triball_1_1 = {{path_triball_1[2][0], path_triball_1[2][1]}, {57, 46.5}};
        move(path_triball_1_1, 279.5, true, false, 0.69, 0.33, 1.4, 45);
        
        // Turn to bar
        Intake::getInstance()->stop();
        turnToAngle(tank_drive_18, 89, 3, false, 2.1);
        Pneumatics::getInstance()->getIntake()->on();
        Intake::getInstance()->toggle_reverse();
        delay(400); // Time for ball to outtake

        // Push first ball over long bar and return
        tank_drive_18.move_with_power((5000.0/12000.0) * 127);
        delay(370);
        tank_drive_18.move_with_power((-5000.0/12000.0) * 127);
        waitUntil(convert::mToIn(tank_drive_18.getOdom().getX()) <= 50+3);
        tank_drive_18.brake();
        Pneumatics::getInstance()->getIntake()->off();
        Intake::getInstance()->stop();
        turnToAngle(tank_drive_18, 0, 1, false, 2.2);
///////////////////////////////////////////////////////////////////////////////////////////

        // Move to get second ball
        Intake::getInstance()->toggle_on();
        vector<vector<double>> path_triball_2 = {{convert::mToIn(tank_drive_18.getOdom().getX()), convert::mToIn(tank_drive_18.getOdom().getY())},
                                                    {convert::mToIn(tank_drive_18.getOdom().getX()), 61}};
        move(path_triball_2, 0, false, false, 0.87, 0.5, 1.12, 45);

        // Move to push second ball over bar
        vector<vector<double>> path_triball_2_1 = {path_triball_2[1], {56, 40}};
        move(path_triball_2_1, 310, true, false, 1, 0.5, 1.12, 45);
        Intake::getInstance()->stop();
        turnToAngle(tank_drive_18, 90, 3, false, 2.1);

        // Push second ball over bar and return
        Pneumatics::getInstance()->getIntake()->on();
        Intake::getInstance()->toggle_reverse();
        delay(450);
        tank_drive_18.move_with_power((5000.0/12000.0) * 127);
        delay(430);
        tank_drive_18.move_with_power((-5000.0/12000.0) * 127);
        waitUntil(convert::mToIn(tank_drive_18.getOdom().getX()) <= 50+3);
        tank_drive_18.brake();
        delay(50);
        Intake::getInstance()->stop();
///////////////////////////////////////////////////////////////////////////////////////////
/** NOTE: Robot thinks it is at X:~45.41 Y:~41.46 H:~ 87.361 */

        // Move to push third ball
        turnToAngle(tank_drive_18, 0, 1, false, 2.1); // 59.45 51.25
        vector<vector<double>> path_triball_3 = {{convert::mToIn(tank_drive_18.getOdom().getX()), convert::mToIn(tank_drive_18.getOdom().getY())},
                                                {58, 50.5}}; // 58, 50.55
        move(path_triball_3, 90, false, false, 0.95, 0.4, 1.44, 5);

        // Push third ball over bar
        Intake::getInstance()->toggle_reverse();
        tank_drive_18.move_with_power((5000.0/12000.0) * 127);
        delay(400);
        tank_drive_18.move_with_power((-5000.0/12000.0) * 127);
        delay(100);
        tank_drive_18.brake();
        Intake::getInstance()->stop();
        Pneumatics::getInstance()->getIntake()->off();

        // Move to bowl
        turnToAngle(tank_drive_18, 45, 2, false, 2.1);
        vector<vector<double>> toBowl = {{convert::mToIn(tank_drive_18.getOdom().getX()), convert::mToIn(tank_drive_18.getOdom().getY())},
                                            {24, 24}};
        move(toBowl, 45, true, false, 0.8, 0.5, 1.44, 45);
        tank_drive_18.move_with_power((-5000.0/12000.0) * 127);
        delay(700);
        tank_drive_18.brake();

        // Align to laod bar
        tank_drive_18.move_with_power((4500.0/12000.0) * 127);
        delay(60);
        tank_drive_18.brake();
        turnToAngle(tank_drive_18, 315, 1, false, 2.1);
///////////////////////////////////////////////////////////////////////////////////////////
        
        // Begin Match loading
        Pneumatics::getInstance()->getWings()->on();
        tank_drive_18.move_with_power((-5000.0/12000.0) * 127);
        delay(450);
        tank_drive_18.brake();

        // Oscillate to bowl 
        for (int i=0; i<bowls; i++) {
            delay(325);
            turnToAngle(tank_drive_18, 265, 5, false, 2.75);
            delay(230);
            turnToAngle(tank_drive_18, 315, 5, false, 2.1);
        }

        // Push triballs away from robot
        tank_drive_18.move_with_power((5000.0/12000.0) * 127);
        delay(300);
        tank_drive_18.brake();
        turnToAngle(tank_drive_18, 135, 7, false, 2.1);
        Intake::getInstance()->toggle_reverse();
///////////////////////////////////////////////////////////////////////////////////////////
        /** POSITION: X:~24.4 Y:~19 H:~135.8 */
        // Bowl to other side
        vector<vector<double>> bowl_path = {{convert::mToIn(tank_drive_18.getOdom().getX()), convert::mToIn(tank_drive_18.getOdom().getY())},
                                                {34, 20}, {57, 12}, {63, 12}};
        move(bowl_path, 50, false, false, 0.79, 0.43, 1.44, 45);
        
        // tank_drive_18.split_tank_with_power((110/600.0)*127, (175/600.0)*127);
        // tank_drive_18.get_motor_group(1).move_velocity(150);
        tank_drive_18.get_motor_group(0).move_velocity(150);
        tank_drive_18.get_motor_group(1).move_velocity(-50);
        delay(1000);
        // // delay(500);
        // tank_drive_18.get_motor_group(1).brake();
        // tank_drive_18.get_motor_group(0).move_velocity(150);
        // delay(500);
        // tank_drive_18.brake();

        Intake::getInstance()->stop();     
        // turnToAngle(tank_drive_18, 100, 2, false, 2.1);   
    }};
	
    // End of auton
    while(!end_auton) {
        pros::delay(20);
    }
    
    if (odom_task.get_state() != E_TASK_STATE_DELETED) {
        odom_task.remove(); // Delete the odom task
    }
    if (auton_task.get_state() != E_TASK_STATE_DELETED){
        auton_task.remove();
    }

    // Stop everything
    // Drive Stop
    tank_drive_18.move_with_power(0);
    // Intake Stop
    Intake::getInstance()->stop();
    ctrl_master.rumble("..-..");
}

void non_win_point_auton() {
    // Non win point auton is the same as win point auton
    win_point_auton();
}
