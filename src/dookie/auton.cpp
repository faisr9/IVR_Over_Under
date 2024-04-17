#include "comp_18/auton.h"

// Auton Methods 

// // Converts inputted tile coordinates, x and y, into meters
// vector<double> vect(double x, double y)
// {
// 	return {(x * .61), (y * .61)};
// }

void move(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor){
	for (auto& vec : moveVec) {
		for (auto& val : vec) { val = convert::inToM(val); }
	}
	followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 600.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

// BEGIN AUTON METHODS //

const double AUTON_DURATION = 45000 - 600; // 45 seconds minus 600 milliseconds for safety of threads exiting
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
    // odom_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);

    // Auton code here
    pros::Task auton_task {[=] {
        // Drop intake
        Intake::getInstance()->toggle_on();
        delay(250); // Robot settle

        // Move to first ball
        // vector<vector<double>> path_triball_1 = {STARTING_POS, {52, 41.23}, {46.1, 54.65}};
        // move(path_triball_1, 350, false, false, 0.8);
        // delay(50);
        // turnToAngle(tank_drive_18, 195, 2.5);
        // Intake::getInstance()->toggle_reverse();
        // delay(250); // Time for ball to outtake
        // Intake::getInstance()->stop();
        // turnToAngle(tank_drive_18, 90, 0.8);
        // Intake::getInstance()->toggle_reverse();

        vector<vector<double>> path_triball_1 = {STARTING_POS, {52, 41.23}, {46.1, 54.65}};
        move(path_triball_1, 0, false, false, 0.8);
        delay(50);
        vector<vector<double>> path_triball_1_1 = {path_triball_1[2], {path_triball_1[2][0], 41}};
        move(path_triball_1_1, 0, true, false, 1.1);
        delay(50);
        turnToAngle(tank_drive_18, 90, 0.8);
        pros::Task intake_delay_start {[=]{ delay(500); Intake::getInstance()->toggle_reverse(); }};
        vector<vector<double>> path_triball_1_2 = {path_triball_1_1[1], {59, path_triball_1_1[1][1]}};
        move(path_triball_1_2, 90, false, false, 1.25);
        delay(50);
        Intake::getInstance()->stop();
        vector<vector<double>> path_triball_1_3 = {{path_triball_1_2[1][0], path_triball_1_2[1][1]}, 
                                                   {path_triball_1_2[0][0]+3, path_triball_1_2[0][1]-1}};
        move(path_triball_1_3, 90, true, false, 1.25);
        delay(50);
        turnToAngle(tank_drive_18, 0, 0.8);
        
        // Move to push second ball
        vector<vector<double>> path_triball_2 = {{46.1, 41}, {46.1, 54.65}};
        move(path_triball_2, 0, false, false, 1.25);
        delay(50);
        turnToAngle(tank_drive_18, 90, 0.8);
        Intake::getInstance()->toggle_reverse();
        tank_drive_18.move_with_power((4000.0/12000.0) * 127);
        double x_point = tank_drive_18.getOdom().getX();
        waitUntil(tank_drive_18.getOdom().getX() >= x_point + convert::inToM(12.9-1.5));
        tank_drive_18.move_with_power(0);
        delay(250);
        Intake::getInstance()->stop();
        vector<vector<double>> path_triball_2_1 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
                                                    convert::mToIn(tank_drive_18.getOdom().getY())}, 
                                                    {46.1+3, convert::mToIn(tank_drive_18.getOdom().getY())}};
        move(path_triball_2_1, 90, true, false, 1.25);
        delay(50);
        turnToAngle(tank_drive_18, 0, 0.8);

        // vector<vector<double>> path_triball_2_1 = {path_triball_2[1], {59, path_triball_2[1][1]}};
        // move(path_triball_2, 90, false, false, 1.25);
        // Intake::getInstance()->stop();
        // Return from pushing second ball
        // vector<vector<double>> path_triball_2_2 = {path_triball_2_1[1], {path_triball_2_1[0][0]+2, path_triball_2_1[1][1]}};
        // move(path_triball_2_1, 90, true, false, 1.25);
        // turnToAngle(tank_drive_18, 0, 0.8);

        // Move to get third ball
        Intake::getInstance()->toggle_on();
        vector<vector<double>> path_triball_3 = {path_triball_2_1[1], {path_triball_2_1[1][0], 62}};
        move(path_triball_3, 0, false, false, 1.25);
        delay(50);
        // Move to push third ball over bar
        vector<vector<double>> path_triball_3_1 = {path_triball_3[1], path_triball_3[0]};
        move(path_triball_3_1, 0, true, false, 1.25);
        Intake::getInstance()->stop();
        turnToAngle(tank_drive_18, 90, 0.8);
        delay(50);
        // Push third ball over bar
        Intake::getInstance()->toggle_reverse();
        x_point = tank_drive_18.getOdom().getX();
        tank_drive_18.move_with_power((4000.0/12000.0) * 127);
        waitUntil(tank_drive_18.getOdom().getX() >= x_point + convert::inToM(12.9-1.5));
        tank_drive_18.move_with_power(0);
        delay(250);
        Intake::getInstance()->stop();
        vector<vector<double>> path_triball_3_2 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
                                                    convert::mToIn(tank_drive_18.getOdom().getY())}, 
                                                    {x_point, convert::mToIn(tank_drive_18.getOdom().getY())}};
        move(path_triball_3_2, 90, false, false, 1.25);
        delay(50);
        turnToAngle(tank_drive_18, 200, 1);

    }};
    // auton_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);
	
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
    // Wings Close
    Pneumatics::getInstance()->getWings()->off();
}

void non_win_point_auton() {
    const double STARTING_X = 18.0;
    const double STARTING_Y = 18.0;
    const double STARTING_ANGLE = 315.0; // 45 degrees south of east
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

// Auton Copies //
// 04/16
/*
pros::Task auton_task {[=] {
    // Drop intake
    Intake::getInstance()->toggle_on();
    delay(250); // Robot settle

    // Move to first ball
    // vector<vector<double>> path_triball_1 = {STARTING_POS, {52, 41.23}, {46.1, 54.65}};
    // move(path_triball_1, 350, false, false, 0.8);
    // delay(50);
    // turnToAngle(tank_drive_18, 195, 2.5);
    // Intake::getInstance()->toggle_reverse();
    // delay(250); // Time for ball to outtake
    // Intake::getInstance()->stop();
    // turnToAngle(tank_drive_18, 90, 0.8);
    // Intake::getInstance()->toggle_reverse();

    vector<vector<double>> path_triball_1 = {STARTING_POS, {52, 41.23}, {46.1, 54.65}};
    move(path_triball_1, 0, false, false, 0.8);
    delay(50);
    vector<vector<double>> path_triball_1_1 = {path_triball_1[2], {path_triball_1[2][0], 41}};
    move(path_triball_1_1, 0, true, false, 1.1);
    delay(50);
    turnToAngle(tank_drive_18, 90, 0.8);
    pros::Task intake_delay_start {[=]{ delay(500); Intake::getInstance()->toggle_reverse(); }};
    vector<vector<double>> path_triball_1_2 = {path_triball_1_1[1], {59, path_triball_1_1[1][1]}};
    move(path_triball_1_2, 90, false, false, 1.25);
    delay(50);
    Intake::getInstance()->stop();
    vector<vector<double>> path_triball_1_3 = {{path_triball_1_2[1][0], path_triball_1_2[1][1]}, 
                                                {path_triball_1_2[0][0]+3, path_triball_1_2[0][1]-1}};
    move(path_triball_1_3, 90, true, false, 1.25);
    delay(50);
    turnToAngle(tank_drive_18, 0, 0.8);
    
    // Move to push second ball
    vector<vector<double>> path_triball_2 = {{46.1, 41}, {46.1, 54.65}};
    move(path_triball_2, 0, false, false, 1.25);
    delay(50);
    turnToAngle(tank_drive_18, 90, 0.8);
    Intake::getInstance()->toggle_reverse();
    tank_drive_18.move_with_power((4000.0/12000.0) * 127);
    double x_point = tank_drive_18.getOdom().getX();
    waitUntil(tank_drive_18.getOdom().getX() >= x_point + convert::inToM(12.9-1.5));
    tank_drive_18.move_with_power(0);
    delay(250);
    Intake::getInstance()->stop();
    vector<vector<double>> path_triball_2_1 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
                                                convert::mToIn(tank_drive_18.getOdom().getY())}, 
                                                {46.1+3, convert::mToIn(tank_drive_18.getOdom().getY())}};
    move(path_triball_2_1, 90, true, false, 1.25);
    delay(50);
    turnToAngle(tank_drive_18, 0, 0.8);

    // vector<vector<double>> path_triball_2_1 = {path_triball_2[1], {59, path_triball_2[1][1]}};
    // move(path_triball_2, 90, false, false, 1.25);
    // Intake::getInstance()->stop();
    // Return from pushing second ball
    // vector<vector<double>> path_triball_2_2 = {path_triball_2_1[1], {path_triball_2_1[0][0]+2, path_triball_2_1[1][1]}};
    // move(path_triball_2_1, 90, true, false, 1.25);
    // turnToAngle(tank_drive_18, 0, 0.8);

    // Move to get third ball
    Intake::getInstance()->toggle_on();
    vector<vector<double>> path_triball_3 = {path_triball_2_1[1], {path_triball_2_1[1][0], 62}};
    move(path_triball_3, 0, false, false, 1.25);
    delay(50);
    // Move to push third ball over bar
    vector<vector<double>> path_triball_3_1 = {path_triball_3[1], path_triball_3[0]};
    move(path_triball_3_1, 0, true, false, 1.25);
    Intake::getInstance()->stop();
    turnToAngle(tank_drive_18, 90, 0.8);
    delay(50);
    // Push third ball over bar
    Intake::getInstance()->toggle_reverse();
    x_point = tank_drive_18.getOdom().getX();
    tank_drive_18.move_with_power((4000.0/12000.0) * 127);
    waitUntil(tank_drive_18.getOdom().getX() >= x_point + convert::inToM(12.9-1.5));
    tank_drive_18.move_with_power(0);
    delay(250);
    Intake::getInstance()->stop();
    vector<vector<double>> path_triball_3_2 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
                                                convert::mToIn(tank_drive_18.getOdom().getY())}, 
                                                {x_point, convert::mToIn(tank_drive_18.getOdom().getY())}};
    move(path_triball_3_2, 90, false, false, 1.25);
    delay(50);
    turnToAngle(tank_drive_18, 200, 1);

}};
*/
