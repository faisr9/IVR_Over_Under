#include "pookie/auton.h"

// Auton Methods 
void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor, double lookAhead, double turnP){
	for (auto& vec : moveVec) {
		for (auto& val : vec) { val = convert::inToM(val); }
	}
	followPath(moveVec, tank_drive_15, angle, isReversed, isSpinAtEnd, false, lookAhead, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, turnP);
}

// BEGIN AUTON METHODS //

bool ENABLE_ABORT = true;
const double AUTON_DURATION = 45000 - 500; // 45 seconds minus 500 milliseconds for safety of threads exiting
void win_point_auton() {

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
            // else if(ctrl_master.get_digital_new_press(BUTTON_B)) {
            //     if (ENABLE_ABORT)
            //         end_auton = true;   
            // }

            pros::delay(20);
        }
    }};
    // odom_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);

    // Auton code here
    
    pros::Task auton_task {[=] {
        // Get Alliance Triballs
        delay(1000);
        Pneumatics::getInstance()->getWings()->on();
        movePID(tank_drive_15, 9.5, 135,1000);
        delay(50);
        turnToAngle(tank_drive_15, 126.5, 2);
        Intake::getInstance()->toggle_on(-600);
        Pneumatics::getInstance()->getWings()->off();
        delay(50);
        movePID(tank_drive_15, 9, 140, 1500);
        delay(50);
        turnToAngle(tank_drive_15, 130, 2);
        vector<vector<double>> toOtherDepot = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getX())}, {2.5*24, 7.8}, {108, 7.5}}; //pickup 1st WP triball along path 
        move(toOtherDepot, 50, false, true, 1.5);
        turnToAngle(tank_drive_15, 50, 2);
        ////////

        // Score WP Triballs
        vector<vector<double>> toGoal = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getX())}, {117,13}, {126,16}, {129, 25}, {130, 27}}; //pickup 2nd WP triball along path
        move(toGoal, 15, false, false, 2,.5);
        turnToAngle(tank_drive_15, 15, 2);
        delay(50);
        Intake::getInstance()->toggle_on();
        delay(50);
        Pneumatics::getInstance()->getIntake()->on();
        for(int i=0; i<4; i++){
            delay(50);
            tank_drive_15.move_with_power(85);
            waitUntil(tank_drive_15.getOdom().getY() >= convert::inToM(32));
            tank_drive_15.move_with_power(0);
            movePID(tank_drive_15, -10, 20-i*5,1000);
        }
        Pneumatics::getInstance()->getIntake()->off();
        ///////

        // Sweep Dookie Sauce
        delay(50);
        // movePID(tank_drive_15, -6, 30, 1000);

        // vector<vector<double>> goBack = {{tank_drive_15.getOdom().getX(), tank_drive_15.getOdom().getY()}, {4.7*24, 24}}; 
        // move(goBack, 315, true, false, 2, .35);
        movePID(tank_drive_15, 5, 0, 1000);
        Intake::getInstance()->stop();
        delay(50);
        turnToAngle(tank_drive_15, 270, 2);
        // tank_drive_15.move_with_power(-55);
        // delay(500);
        // tank_drive_15.move_with_power(0);
        // delay(50);
        Intake::getInstance()->toggle_on(-600);
        Pneumatics::getInstance()->getWings()->on();
        vector<vector<double>> sweep = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())},
                                            {108,22.5}, {85, 40}, {87, 67}, {90, 64}, {115, 64}}; //pickup 2nd WP triball along path
        move(sweep, 90, false, true, 2, .35);
        delay(50);
        Intake::getInstance()->toggle_on();
        // ram in front of goal
        for(int i=0; i<3; i++){
            delay(50);
            tank_drive_15.move_with_power(85);
            waitUntil(tank_drive_15.getOdom().getX() >= convert::inToM(122));
            tank_drive_15.move_with_power(0);
            movePID(tank_drive_15, -10, 90,1000);   
        }
        /////

        // To Climb Bar (WP END)
        Intake::getInstance()->toggle_on(-600);
        delay(50);
        turnToAngle(tank_drive_15, 180, 2);
        vector<vector<double>> toClimb = {{convert::mToIn(tank_drive_15.getOdom().getX()), convert::mToIn(tank_drive_15.getOdom().getY())}, {108,30}, {80, 31}}; //pickup 2nd WP triball along path
        move(toClimb, 280, false, false, 2, .35);
        turnToAngle(tank_drive_15, 280, 2);
        // tank_drive_15.move_with_power(55);
        // delay(400);
        // tank_drive_15.move_with_power(0);
        // movePID(tank_drive_15, 4, 290, 1000);
        tank_drive_15.split_tank_with_power(28,0);
        delay(500);
        tank_drive_15.move_with_power(0);
        ctrl_master.rumble(".");
        // delay(100000);
    }};
    // auton_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);
	
    // End of auton
    int cnt1 = 0;
    int cnt2 = 0;
    int cnt3 = 0;
    while(!end_auton) {
        if(tank_drive_15.getOdom().getX() > convert::inToM(1.8*24)&& !cnt1){
            cnt1++;
            Intake::getInstance()->stop();
        }
        if(tank_drive_15.getOdom().getX() >= convert::inToM(96) && tank_drive_15.getY() >= convert::inToM(9) && !cnt2){
            Pneumatics::getInstance()->getWings()->on();
            cnt2++;
        }
        if(tank_drive_15.getOdom().getX() >= convert::inToM(115) && tank_drive_15.getY() >= convert::inToM(21) && !cnt3){
            Pneumatics::getInstance()->getWings()->off();
            cnt3++;
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

void non_win_point_auton() {

    const double STARTING_X = 18.0;
    const double STARTING_Y = 18.0;
    const double STARTING_ANGLE = 315.0; // 45 degrees south of east
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
    odom_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);

    // Auton code here
    pros::Task auton_task {[=] {
		
    }};
    auton_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);

    // End of auton
    while(!end_auton) {
        pros::delay(20);
    }
    odom_task.remove(); // Delete the odom task
    auton_task.remove(); // Delete the auton task
    // Stop everything
    // Drive Stop
    // Intake Stop
    // Wings Close
}