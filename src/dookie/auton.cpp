#include "comp_18/auton.h"

// Auton Methods 

// // Converts inputted tile coordinates, x and y, into meters
// vector<double> vect(double x, double y)
// {
// 	return {(x * .61), (y * .61)};
// }

void turn_comp(double angle)
{
	turnToAngle(tank_drive_18, angle, 2.0, false, 1); // p=1.12 // turndegtolerance=3 //time 150
}

void move_slw_comp(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
	double speedfactor = 3.87;
	followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

// vector<double> vectOff_comp(double x, double y)
// {
// 	return {(.61 * 2.2) + (x * .61), (.61 * .5) + (y * .61)};
// }

void move_comp(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
	double speedfactor = 2.7;
	followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

// BEGIN AUTON METHODS //

const double AUTON_DURATION = 45000 - 600; // 45 seconds minus 500 milliseconds for safety of threads exiting
void win_point_auton() {

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