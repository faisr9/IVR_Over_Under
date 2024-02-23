#include "comp_15/auton.h"
LinkHelper* comp15link = LinkHelper::createInstance(16, E_LINK_TX);

void auton_15(double auton_duration_time_millis, bool climb) {

    pros::lcd::set_text(1, "Hiiiiii running auton");
    // (0, 0) corner is where positive x is going to other side of field and positive y is same side of field
    const double kSTARTING_X = 0.5;
    const double kSTARTING_Y = 0.3;
    const double kSTARTING_ANGLE = 60.0;

    const double kAUTON_START_TIME = pros::millis();
    const double kAUTON_CODE_DURATION = auton_duration_time_millis - 500;

    drive.getOdom().initTracker(kSTARTING_X, kSTARTING_Y, kSTARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::delay(50);
		}
	}}; // lambda function with a task

    Pneumatics::getInstance()->getFloorBrake()->on();
    pros::delay(500);
    // release intake and catapult
    Intake::getInstance()->set_power(127);
    pros::delay(250);
    // kickstand down
    Intake::getInstance()->set_power(0);
    delay(1500);

    // start shooting portion of autonomous

	// const int numCycles	= 3;
	// std::string numCyclesStr = std::to_string(numCycles);
	// send the number of cycles to the other robot
	// comp15link->sendMsg(std::to_string(numCycles));

    pros::Task shooting_task{[=] {
        bool hasFired = false;
        CompetitionCatapult::getInstance()->set_cata_mode("P");
        pros::delay(3000); // delay so jonah can place the triball
	    int cycleCounter = 0;
        while (1) {
            // doinker down
            DoinkerClass::getInstance()->move(DoinkerClass::DOWN); // did not go past here
            // small delay
            pros::delay(600);
            // doinker up
            DoinkerClass::getInstance()->move(DoinkerClass::UP);
            // medium delay
            pros::delay(1000);

            if (!hasFired) {
                comp15link->waitForNotify(8000);
                hasFired = true;
            }
            // wait for comp18 to get to position / cycle triball into goal
            // comp15link->waitForNotify(???);
            
            // fire catapult and put it back down
            CompetitionCatapult::getInstance()->set_cata_mode("RP");
            cycleCounter++;
            // wait for human player
            pros::delay(2500);
        }
	}}; // lambda function with a task

    // comp15link->sendMsg("end_auton");

    // create while loop that will exit after kMAX_SHOOTING_TIME/1000 seconds
    // const int kMAX_SHOOTING_TIME = 30000;
    // const double start_shoot_time = pros::millis();
    // create while loop that will exit right before the end of auton
    // const int kMAX_SHOOTING_TIME = 30000;
    // const double start_shoot_time = pros::millis();
    // no need to check if task has terminated because it can't
    bool set_rumble = false;
    const double kCLIMB_TIME = 4000;
    const double kSHOOT_ABORT_TIME = (climb) ? kAUTON_START_TIME + kAUTON_CODE_DURATION - kCLIMB_TIME : kAUTON_START_TIME + kAUTON_CODE_DURATION;
    while (pros::millis() < kSHOOT_ABORT_TIME) {
        if (pros::millis() > kSHOOT_ABORT_TIME - 2000 && !set_rumble) {
            ctrl_master.rumble("--");
            set_rumble = true;
        }
        pros::delay(100);
    }
    shooting_task.suspend();
    
    // deconstruction and reset systems
    ctrl_master.rumble(" ");
    Pneumatics::getInstance()->getFloorBrake()->off();
    CompetitionCatapult::getInstance()->set_cata_mode("R");
    pros::lcd::set_text(1, "Done with the path!");
    if (!climb) odom_task.suspend(); // doinker will take program cycles to move so make sure odom is suspended first
    DoinkerClass::getInstance()->move(DoinkerClass::UP);
    
    
    if (!climb) {
        pros::lcd::set_text(7, "SUCCESSFULLY FINISHED AUTON CODE");
        return; 
    }
    // go climb

    pros::Task path_task{[=] {
        std::vector<std::vector<double>> to_climb_bar = {{kSTARTING_X, kSTARTING_Y}, {1.2, 0.25}, {2.4, 0.2}};
        followPath(to_climb_bar, drive, 90, false, false, false, 0.5, 3.0, 220.0, 450.0, 220);
	}}; // lambda function with a task

    const int kCATA_TIME = 1000;
    const double kPATH_MAX_TIME = kCLIMB_TIME - kCATA_TIME;
    double start_time = pros::millis();
    while (pros::millis() < start_time + kPATH_MAX_TIME && path_task.get_state() != pros::E_TASK_STATE_DELETED) {
        if (pros::millis() < start_time + (kPATH_MAX_TIME / 2)) {
            CompetitionCatapult::getInstance()->set_cata_mode("R");
            // in case cata task was busy when it was told to come up
        }
    
        pros::delay(100);
    }

    // if the path is still going
    if (path_task.get_state() != pros::E_TASK_STATE_DELETED && path_task.get_state() != pros::E_TASK_STATE_INVALID) {
        // if call suspend on deleted task program will crash! (data abort)
        // this is why program would only crash if program successfully completed
        path_task.suspend();
        stopMotors(drive);
    }

    CompetitionCatapult::getInstance()->set_cata_mode("PR");

    // pros::delay(350);

    // CompetitionCatapult::getInstance()->set_cata_mode("R");

    odom_task.suspend();
}









void skills_15(bool driver) {
    const double starting_time = pros::millis();

    const double kSKILLS_DURATION = 45000; // NEED TO CHANGE THIS LATER, 40000 FOR TESTING

    const double kBOWL_TIME = 25000; // time we are driving the robot and pushing things in for
    const double kBACK_FORTHS = 1; // we will shove in and back out 3 times

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

    
    // Intake::getInstance()->set_power(127);
    // pros::delay(500);
    // Intake::getInstance()->set_power(0);

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
            // turn back so the human player can place another triball
            turnToAngle(drive, kTURN_BACK_ANGLE, 5.0, false, 1.9, 80);
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
    turnToAngle(drive, 120, 3.0, false, 1.9);
    // turnToAngle(drive, 90, 3.0, false, 1.9);

    // turnToAngle(drive, kSTART_ANGLE, 3.0);
    // while (1) {
    //     pros::delay(100);
    // }


    moveMotors(drive, -150, -150);
    pros::delay(150);
    stopMotors(drive);
    // turnToAngle(drive, kSTART_ANGLE, 3.0, false, 2.1);

    /* REVERSED ONLY
    // task to drive to the bowl
    pros::Task to_bowl_pos_task{[=] {
        
        // turnToAngle(drive, 270.0, 3.0);
        vector<vector<double>> path_to_goal = {{drive.getOdom().getX(), drive.getOdom().getY()}, {0.7, 0.35}, {2.7, 0.3}};

        // reverse
        followPath(path_to_goal, drive, 270, true, false, false, 0.5, 5.0);

        // followPath(path_to_goal, drive, 90, false, false, false, 0.5, 5.0);

        vector<vector<double>> back_up = {path_to_goal.back(), {2.4, 0.3}};
        followPath(back_up, drive, 85, false, true, false, 0.5, 5.0); // 83 so the robot turns to the left, 
    }};


    // wait until task is over
    while (to_bowl_pos_task.get_state() != pros::E_TASK_STATE_DELETED) {
        // while path is not done
        // have this here in case we want to extend/retract wings during path

        // this won't work bc chance we close wing in on triball and make a V and control > 1 triball
        // if (!wings_put_in && drive.getOdom().getX() > 1.0) {
        //     // wait until we're far enough through path to put wings in
        //     Pneumatics::getInstance()->getWings()->off();
        //     wings_put_in = true;
        // }
        pros::delay(100);
    }
    */
    // no need to call suspend because it finishes on its own
    // Pneumatics::getInstance()->getWings()->on();
    // first push in

    pros::Task to_goal_task {[=] {
        // need to be sure to account for width of triballs and to turn early // 2.94 0.28
        // vector<vector<double>> push_in = {{drive.getOdom().getX(), drive.getOdom().getY()}, {0.7, 0.35}, {2.6, 0.35}, {3.2, 0.5}, {3.3, 0.9}}; // was 2.5 and 3.0 earlier!!!
        // vector<vector<double>> push_in = {{drive.getOdom().getX(), drive.getOdom().getY()}, {0.7, 0.31}, {2.8, 0.31}, {3.3, 0.5}, {3.3, 0.9}}; // was 2.5 and 3.0 earlier!!!
        vector<vector<double>> push_in = {{drive.getOdom().getX(), drive.getOdom().getY()}, {0.7, 0.38}, {3.0, 0.38}, {3.4, 0.5}, {3.3, 0.9}}; // was 2.5 and 3.0 earlier!!!

        followPath(push_in, drive, 0, false, false, false, 0.5, 4.0, 200.0, 450.0, 150.0, false, 1.9);
    }};

    // TODO: make it so wings go out before (but then back in) before going under
    // then come back out after done and go back in after 45

    const int kWINGS_IN_ANGLE = 45; // take the wings in once the robot has turned a certain amount
    Piston* pneumatic_wings = Pneumatics::getInstance()->getWings();
    while (to_goal_task.get_state() != pros::E_TASK_STATE_DELETED) {

        // with this setup right wing is diabled so don't have to worry about it and hitting the wall
        if (drive.getOdom().getX() < 1.7 && drive.getX() > 1.5) {
            pneumatic_wings->off();
        } else {
            pneumatic_wings->on(); // on
        }

        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
            to_goal_task.suspend();
            stopMotors(drive);
            return;
        }

        pros::delay(50);
    }

    // go back and forth some number of times
    for (int i = 0; i < kBACK_FORTHS; i++) {
        vector<vector<double>> go_back = {{drive.getOdom().getX(), drive.getOdom().getY()}, {3.3, 0.7}};
        followPath(go_back, drive, 0, true, true);
        pros::delay(100);
        moveMotors(drive, 140, 140);

        // TODO: Make it so wing comes in when past y = 1.0? so it doesn't get stuck
        pros::delay(1200);
    }

    // final drive back, make sure there's time for this bc any triballs being touched at end don't count
    moveMotors(drive, -300, -300);
    pneumatic_wings->off();
    pros::delay(500);
    stopMotors(drive);

    // we should decide if taking the wings in at the end is a good idea or not (risk of touching triballs)

    // Pneumatics::getInstance()->getWings()->off();
    odom_task.suspend();
}