#include "comp_18/auton.h"

// Auton Methods 

// // Converts inputted tile coordinates, x and y, into meters
// vector<double> vect(double x, double y)
// {
// 	return {(x * .61), (y * .61)};
// }

void move(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor){
	for (auto& vec : moveVec) {
		for (auto& val : vec) { convert::inToM(val); }
	}
	followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 600.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

// BEGIN AUTON METHODS //

bool ENABLE_ABORT = true;
const double AUTON_DURATION = 45000 - 600; // 45 seconds minus 500 milliseconds for safety of threads exiting
void win_point_auton() {

    const double STARTING_X = 0;
    const double STARTING_Y = 0;
    const double STARTING_ANGLE = 0; // 45 degrees south of east
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
            else if(ctrl_master.get_digital_new_press(BUTTON_B)) {
                if (ENABLE_ABORT)
                    end_auton = true;   
            }

            pros::delay(20);
        }
    }};
    // odom_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);

    // Auton code here
    pros::Task auton_task {[=] {
        Intake::getInstance()->set_rpm(300);
		move({{0,0},{.61,0}}, 0, false, false);
		pros::delay(50);
		turnToAngle(tank_drive_18, 180, 0.5,false);
		pros::delay(50);
		move({{.61,0},{0,0}}, 180, false, false);
		pros::delay(2000);
    }};
    // auton_task.set_priority(TASK_PRIORITY_MEDIUM_HIGH);
	
    // End of auton
    while(!end_auton) {
        pros::delay(20);
    }
    
    if (odom_task.get_state() == E_TASK_STATE_RUNNING || odom_task.get_state() == E_TASK_STATE_READY || odom_task.get_state() == E_TASK_STATE_SUSPENDED) {
        odom_task.remove(); // Delete the odom task
    }
    if (auton_task.get_state() == E_TASK_STATE_RUNNING) {
        auton_task.remove(); // Delete the auton task
    }
    // auton_task.remove(); // Delete the auton task
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