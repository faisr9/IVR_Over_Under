#include "comp_15/auton.h"

LinkHelper* comp15link = LinkHelper::createInstance(16, E_LINK_TX);

void skills_15(bool driver) {
    const double starting_time = pros::millis();

    const double kSKILLS_DURATION = (driver) ? 55000 : 60000; // NEED TO CHANGE THIS LATER, 40000 FOR TESTING

    const double kBOWL_TIME = 25000; // time we are driving the robot and pushing things in for
    const int kBACK_FORTHS = 3; // we will shove in and back out 3 times

    const double kSTART_ANGLE = 315.0;
    const double kTURN_BACK_ANGLE = 300.0;
    vector<double> kSTARTING_POS = {0.53, 0.5};

    drive.getOdom().initTracker(kSTARTING_POS[0], kSTARTING_POS[1], kSTART_ANGLE);
    pros::delay(50);
    pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::delay(50);
		}
	}}; // lambda function with a task
    Pneumatics::getInstance()->getWings()->on();
    Intake::getInstance()->set_power(-100);
    pros::delay(500);
    Intake::getInstance()->set_power(0);

    // load until need to go bc of kBOWL_TIME
    pros::Task spinning_task{[=] {
        int cycles = 0;
        const double kHP_WAIT_TIME = 0;
        const double kHP_FIRST_WAIT_TIME = 0;
        const int turn_amount = 45;
        Pneumatics::getInstance()->getWings()->on();
        pros::delay(kHP_FIRST_WAIT_TIME);

        while (1) {
            // whack triball out of match load zone
            turnToAngle(drive, kTURN_BACK_ANGLE - turn_amount, 6.0, false, 3.3, 10); // more p because triball is in the way and don't care as much about precision
            pros::delay(500);
            // turn back so the human player can place another triball
            turnToAngle(drive, kTURN_BACK_ANGLE, 5.0, false, 2.2, 400);
            pros::delay(kHP_WAIT_TIME);
            cycles++;
        }
    }};


    while (pros::millis() - starting_time < (kSKILLS_DURATION - kBOWL_TIME)) {
        if (driver && ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
            // abort code for driver control
            spinning_task.suspend();
            odom_task.suspend();
            stopMotors(drive);
            return;
        }

        pros::delay(50);
    }

    // stop spinning to get match loads and get ready for next part of auton
    spinning_task.suspend();
    
    if (driver) {
        odom_task.suspend();
        stopMotors(drive);
        return;
    }

    Pneumatics::getInstance()->getWings()->off();
    turnToAngle(drive, 120, 3.0);

    moveMotors(drive, -150, -150);
    pros::delay(150);
    stopMotors(drive);

    pros::Task to_goal_task {[=] {
        Intake::getInstance()->set_power(-127);
        
        vector<vector<double>> push_in = {{drive.getOdom().getX(), drive.getOdom().getY()}, {0.7, 0.4}, {2.94, 0.4}, {3.4, 0.6}, {3.4, 0.75}}; // was 2.5 and 3.0 earlier!!!

        followPath(push_in, drive, 0, false, false, false, 0.5, 2.0, 150.0, 450.0, 100.0, false, 2.2);
    }};

    // TODO: make it so wings go out before (but then back in) before going under
    // then come back out after done and go back in after 45

    Piston* pneumatic_wings = Pneumatics::getInstance()->getWings();
    while (to_goal_task.get_state() != pros::E_TASK_STATE_DELETED) {

        // with this setup right wing is diabled so don't have to worry about it and hitting the wall
        if (drive.getOdom().getX() < 1.7 && drive.getX() > 1.5) {
            pneumatic_wings->off();
        } else {
            pneumatic_wings->on(); // on
        }

        // abort for testing (commented out to avoid disaster)
        // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
        //     to_goal_task.suspend();
        //     stopMotors(drive);
        //     Intake::getInstance()->set_power(0);
        //     return;
        // }

        pros::delay(50);
    }

    // go back and forth some number of times
    const double kGO_BACK_TIMEOUT = 4000;
    for (int i = 0; i < kBACK_FORTHS; i++) {
        if (drive.getOdom().getY() >= 0.82) {

            const double go_back_start_time = pros::millis();
            pros::Task go_back_task {[=] {
                vector<vector<double>> go_back = {{drive.getOdom().getX(), drive.getOdom().getY()}, {3.4, 0.72}};

                followPath(go_back, drive, 0, true, true);
            }};

            while (go_back_task.get_state() != pros::E_TASK_STATE_DELETED) {
                if (pros::millis() > go_back_start_time + kGO_BACK_TIMEOUT) {
                    go_back_task.suspend();
                    turnToAngle(drive, 5, 3.0, false, 2.5);
                    break;
                }
            }
        } else {
            turnToAngle(drive, 5, 3.0, false, 2.8, 250);
        }
        pros::delay(100);
        moveMotors(drive, 350, 350);

        pros::delay(1200);
    }

    // final drive back, make sure there's time for this bc any triballs being touched at end don't count
    moveMotors(drive, -200, -200);
    pneumatic_wings->off();
    pros::delay(500);
    Intake::getInstance()->set_power(0);
    stopMotors(drive);

    // we should decide if taking the wings in at the end is a good idea or not (risk of touching triballs)
    // Pneumatics::getInstance()->getWings()->off();

    odom_task.suspend();
}