#include "dookie/auton.h"

// Auton Methods 

// // Converts inputted tile coordinates, x and y, into meters
// vector<double> vect(double x, double y)
// {
// 	return {(x * .61), (y * .61)};
// }

/* Avaible Auton Methods 
turnToAngle
turnPID
latPID (fwd, rev movements only)
movePID move and turn PID
*/

// Used to step through auton
void ctrlerHold() {
    while(1) {
        if (ctrl_master.get_digital(BUTTON_X)) {
            break;
        }
        pros::delay(20);
    }
}

void move(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor,  double lookAhead, double turnP){
	for (auto& vec : moveVec) {
		for (auto& val : vec) { val = convert::inToM(val); }
	}
	followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, lookAhead, 3.0, 200.0 / speedfactor, 600.0 / speedfactor, 40.0 / speedfactor, false, turnP);
}

// BEGIN AUTON METHODS //

const double AUTON_DURATION = 100000 - 600; // 45 seconds minus 600 milliseconds for safety of threads exiting
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
        double x_point = 0.0;   
        // Drop intake
        Intake::getInstance()->toggle_on();
        delay(500); // Robot settle
///////////////////////////////////////////////////////////////////////////////////////////
        // Move to get first ball
        vector<vector<double>> path_triball_1 = {STARTING_POS, {56, 37.23}, {46.1, 52.65}};
        move(path_triball_1, 342, false, false, 0.69);

        // Back up to push first ball
        vector<vector<double>> path_triball_1_1 = {{path_triball_1[2][0], path_triball_1[2][1]}, {57, 46.5}};
        move(path_triball_1_1, 273, true, false, 0.69, 0.33, 1.4);
        
        // Turn to bar
        Intake::getInstance()->stop();
        
        // turnToAngle(tank_drive_18, 90, 1, false, 2.1);
        Intake::getInstance()->toggle_reverse();
        delay(650); // Time for ball to outtake
        
        // // Push first ball over long bar and return
        // x_point = tank_drive_18.getOdom().getX();
        // tank_drive_18.move_with_power((5000.0/12000.0) * 127);
        // delay(230);
        // tank_drive_18.move_with_power((-5000.0/12000.0) * 127);
        // waitUntil(tank_drive_18.getOdom().getX() <= x_point - convert::inToM(6.4));
        // tank_drive_18.move_with_power(0);
        // Intake::getInstance()->stop();
        // turnToAngle(tank_drive_18, 0, 0.8, false, 2.2);
///////////////////////////////////////////////////////////////////////////////////////////

//         // Move to get second ball
//         Intake::getInstance()->toggle_on();
//         vector<vector<double>> path_triball_2 = {{convert::mToIn(tank_drive_18.getOdom().getX()), convert::mToIn(tank_drive_18.getOdom().getY())},
//                                                     {convert::mToIn(tank_drive_18.getOdom().getX()), 62.5}};
//         move(path_triball_2, 0, false, false, 1.25);
//         delay(50);
//         // Move to push second ball over bar
//         vector<vector<double>> path_triball_2_1 = {path_triball_2[1], {60, 46.5}};
//         move(path_triball_2_1, 311, true, false, 1.25);
//         Intake::getInstance()->stop();
//         turnToAngle(tank_drive_18, 90, 0.8);
//         delay(50);
//         // Push second ball over bar
//         Intake::getInstance()->toggle_reverse();
//         delay(650);
//         x_point = tank_drive_18.getOdom().getX();
//         tank_drive_18.move_with_power((3000.0/12000.0) * 127);
//         delay(350);
//         tank_drive_18.move_with_power((-3500.0/12000.0) * 127);
//         waitUntil(tank_drive_18.getOdom().getX() <= x_point - convert::inToM(4.25));
//         tank_drive_18.move_with_power(0);
//         delay(50);
//         Intake::getInstance()->stop();
//         // // Return from pushing second ball
//         // vector<vector<double>> path_triball_3_2 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
//         //                                             convert::mToIn(tank_drive_18.getOdom().getY())}, 
//         //                                             {convert::mToIn(x_point), convert::mToIn(tank_drive_18.getOdom().getY())}};
//         // move(path_triball_3_2, 90, true, false, 1.25);
//         // delay(50);
// // ///////////////////////////////////////////////////////////////////////////////////////////
// /** NOTE: Robot thinks it is at X:~45.41 Y:~41.46 H:~ 87.361 */
//         // Move to get third ball at angle
//         turnToAngle(tank_drive_18, 0, 0.8); // 59.45 51.25
//         Intake::getInstance()->toggle_on();
//         vector<vector<double>> path_triball_4 = {{convert::mToIn(tank_drive_18.getOdom().getX()), convert::mToIn(tank_drive_18.getOdom().getY())},
//                                                  {55.5, 56.5}};
//         move(path_triball_4, 90, false, false, 1.45, 0.32);
//         delay(50);
//         Intake::getInstance()->stop();
//         delay(50);
//         // Push third ball over bar
//         Intake::getInstance()->toggle_reverse();
//         x_point = tank_drive_18.getOdom().getX();
//         tank_drive_18.move_with_power((3000.0/12000.0) * 127);
//         delay(400);
//         tank_drive_18.move_with_power((-3500.0/12000.0) * 127);
//         waitUntil(tank_drive_18.getOdom().getX() <= x_point - convert::inToM(7));
//         tank_drive_18.move_with_power(0);
//         delay(250);
//         Intake::getInstance()->stop();






        // // Return from pushing third ball
        // vector<vector<double>> path_triball_4_1 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
        //                                             convert::mToIn(tank_drive_18.getOdom().getY())}, 
        //                                             {convert::mToIn(x_point)-10, convert::mToIn(tank_drive_18.getOdom().getY())}};
        // move(path_triball_4_1, 90, true, false, 1.25);
        // delay(50);
        // turnToAngle(tank_drive_18, 205, 0.8);
        
        // Move to push second ball
        // vector<vector<double>> path_triball_2 = {{46.1, 41}, {46.1, 54.65}};
        // move(path_triball_2, 0, false, false, 1.25);
        // delay(50);
        // turnToAngle(tank_drive_18, 90, 0.8);
        // Intake::getInstance()->toggle_reverse();
        // // Push second ball over long bar
        // tank_drive_18.move_with_power((4000.0/12000.0) * 127);
        // double x_point = tank_drive_18.getOdom().getX();
        // waitUntil(tank_drive_18.getOdom().getX() >= x_point + convert::inToM(12.9-1.5));
        // tank_drive_18.move_with_power(0);
        // delay(250);
        // Intake::getInstance()->stop();
        // // Return from pushing second ball
        // vector<vector<double>> path_triball_2_1 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
        //                                             convert::mToIn(tank_drive_18.getOdom().getY())}, 
        //                                             {46.1+3, convert::mToIn(tank_drive_18.getOdom().getY())}};
        // move(path_triball_2_1, 90, true, false, 1.25);
        // delay(50);
        // turnToAngle(tank_drive_18, 0, 0.8);


        // turnToAngle(tank_drive_18, 200, 1);
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

// 04/17
// Ball two to push two across the bar at the same time
/*
        // Move to get second ball
        Intake::getInstance()->toggle_on();
        vector<vector<double>> path_triball_2 = {path_triball_1_3[1], {path_triball_1_3[1][0], 62.5}};
        move(path_triball_2, 0, false, false, 1.25);
        delay(50);
        // Move to push second ball over bar
        Pneumatics::getInstance()->getWings()->on();
        double y_point = tank_drive_18.getOdom().getY();
        tank_drive_18.move_with_power((-2700.0/12000.0) * 127);
        waitUntil(tank_drive_18.getOdom().getY() <= y_point - convert::inToM(4.75));
        tank_drive_18.move_with_power(0);
        delay(50);
        // vector<vector<double>> path_triball_2_1 = {path_triball_2[1], {path_triball_2[1][0], 61.5}};
        // move(path_triball_2_1, 0, true, false, 1.25);
        Intake::getInstance()->stop();
        turnToAngle(tank_drive_18, 90, 0.8);
        delay(50);
        // Push second ball over bar
        double x_point = tank_drive_18.getOdom().getX();
        tank_drive_18.move_with_power((2700.0/12000.0) * 127);
        pros::Task intake_delay_start {[=]{ delay(200); Intake::getInstance()->toggle_reverse(); }};
        waitUntil(tank_drive_18.getOdom().getX() >= x_point + convert::inToM(12.9-1.2));
        tank_drive_18.move_with_power(0);
        delay(450);
        Intake::getInstance()->stop();
        // Return from pushing second ball
        vector<vector<double>> path_triball_3_2 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
                                                    convert::mToIn(tank_drive_18.getOdom().getY())}, 
                                                    {convert::mToIn(x_point), convert::mToIn(tank_drive_18.getOdom().getY())}};
        move(path_triball_3_2, 90, true, false, 1.25);
        delay(50);
*/

// 04/18
//     pros::Task auton_task {[=] {
//         // Drop intake
//         Intake::getInstance()->toggle_on();
//         delay(500); // Robot settle
// ///////////////////////////////////////////////////////////////////////////////////////////
//         // Move to get first ball
//         vector<vector<double>> path_triball_1 = {STARTING_POS, {53, 39.23}, {46.1, 54.65}};
//         move(path_triball_1, 350, false, false, 0.8);
//         delay(50);
//         // Back up to push first ball
//         vector<vector<double>> path_triball_1_1 = {path_triball_1[2], {path_triball_1[2][0], 41}};
//         move(path_triball_1_1, 0, true, false, 1.1);
//         delay(50);
//         Intake::getInstance()->stop();
//         turnToAngle(tank_drive_18, 90, 0.8);
//         pros::Task intake_delay_start_1 {[=]{ delay(400); Intake::getInstance()->toggle_reverse(); }};
//         // Push first ball over long bar
//         vector<vector<double>> path_triball_1_2 = {path_triball_1_1[1], {59, path_triball_1_1[1][1]}};
//         move(path_triball_1_2, 90, false, false, 1.25);
//         delay(50);
//         Intake::getInstance()->stop();
//         // Return from pushing first ball
//         vector<vector<double>> path_triball_1_3 = {path_triball_1_2[1], {path_triball_1_2[0][0]+3, path_triball_1_2[0][1]}};
//         move(path_triball_1_3, tank_drive_18.getOdom().getHeading(), true, false, 1.2);
//         delay(50);
//         turnToAngle(tank_drive_18, 0, 0.8);
// ///////////////////////////////////////////////////////////////////////////////////////////
//         // Move to get second ball
//         Intake::getInstance()->toggle_on();
//         vector<vector<double>> path_triball_2 = {path_triball_1_3[1], {path_triball_1_3[1][0], 62.5}};
//         move(path_triball_2, 0, false, false, 1.25);
//         delay(50);
//         // Move to push second ball over bar
//         vector<vector<double>> path_triball_2_1 = {path_triball_2[1], path_triball_2[0]};
//         move(path_triball_2_1, 0, true, false, 1.25);
//         Intake::getInstance()->stop();
//         turnToAngle(tank_drive_18, 90, 0.8);
//         delay(50);
//         // Push second ball over bar
//         double x_point = tank_drive_18.getOdom().getX();
//         tank_drive_18.move_with_power((4000.0/12000.0) * 127);
//         pros::Task intake_delay_start {[=]{ delay(325); Intake::getInstance()->toggle_reverse(); }};
//         waitUntil(tank_drive_18.getOdom().getX() >= x_point + convert::inToM(12.9-1.2));
//         tank_drive_18.move_with_power(0);
//         delay(400);
//         Intake::getInstance()->stop();
//         // Return from pushing second ball
//         vector<vector<double>> path_triball_3_2 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
//                                                     convert::mToIn(tank_drive_18.getOdom().getY())}, 
//                                                     {convert::mToIn(x_point), convert::mToIn(tank_drive_18.getOdom().getY())}};
//         move(path_triball_3_2, 90, true, false, 1.25);
//         delay(50);
// ///////////////////////////////////////////////////////////////////////////////////////////
// /** NOTE: Robot thinks it is at X:~47.65 Y:~40.538 H:~ 89.661 */
//         // Move to get third ball at angle
//         turnToAngle(tank_drive_18, 48, 1);
//         Intake::getInstance()->toggle_on();
//         vector<vector<double>> path_triball_4 = {path_triball_3_2[1], {61.2, 50.7}};
//         move(path_triball_4, 45, false, false, 1.45);
//         delay(50);
//         Intake::getInstance()->stop();
//         // Turn to push third ball over bar
//         turnToAngle(tank_drive_18, 90, 0.8);
//         delay(50);
//         // Push third ball over bar
//         pros::Task intake_delay_start_2 {[=]{ delay(300); Intake::getInstance()->toggle_reverse(); }};
//         x_point = tank_drive_18.getOdom().getX();
//         tank_drive_18.move_with_power((-2000.0/12000.0) * 127);
//         waitUntil(tank_drive_18.getOdom().getX() <= x_point - convert::inToM(4));
//         tank_drive_18.move_with_power(0);
//         delay(500);
//         x_point = tank_drive_18.getOdom().getX();
//         tank_drive_18.move_with_power((2000.0/12000.0) * 127);
//         waitUntil(tank_drive_18.getOdom().getX() >= x_point + convert::inToM(4));
//         tank_drive_18.move_with_power(0);
//         delay(250);
//         Intake::getInstance()->stop();
//         // Return from pushing third ball
//         vector<vector<double>> path_triball_4_1 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
//                                                     convert::mToIn(tank_drive_18.getOdom().getY())}, 
//                                                     {convert::mToIn(x_point)-10, convert::mToIn(tank_drive_18.getOdom().getY())}};
//         move(path_triball_4_1, 90, true, false, 1.25);
//         delay(50);
//         turnToAngle(tank_drive_18, 205, 0.8);











        
//         // Move to push second ball
//         // vector<vector<double>> path_triball_2 = {{46.1, 41}, {46.1, 54.65}};
//         // move(path_triball_2, 0, false, false, 1.25);
//         // delay(50);
//         // turnToAngle(tank_drive_18, 90, 0.8);
//         // Intake::getInstance()->toggle_reverse();
//         // // Push second ball over long bar
//         // tank_drive_18.move_with_power((4000.0/12000.0) * 127);
//         // double x_point = tank_drive_18.getOdom().getX();
//         // waitUntil(tank_drive_18.getOdom().getX() >= x_point + convert::inToM(12.9-1.5));
//         // tank_drive_18.move_with_power(0);
//         // delay(250);
//         // Intake::getInstance()->stop();
//         // // Return from pushing second ball
//         // vector<vector<double>> path_triball_2_1 = {{convert::mToIn(tank_drive_18.getOdom().getX()), 
//         //                                             convert::mToIn(tank_drive_18.getOdom().getY())}, 
//         //                                             {46.1+3, convert::mToIn(tank_drive_18.getOdom().getY())}};
//         // move(path_triball_2_1, 90, true, false, 1.25);
//         // delay(50);
//         // turnToAngle(tank_drive_18, 0, 0.8);


//         // turnToAngle(tank_drive_18, 200, 1);

//     }};