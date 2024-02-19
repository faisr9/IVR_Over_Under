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

void skills_15() {
    const double starting_time = pros::millis();

    const double kSKILLS_DURATION = 40000; // NEED TO CHANGE THIS LATER, 40000 FOR TESTING

    const double kBOWL_TIME = 25000; // time we are driving the robot and pushing things in for
    const double kBACK_FORTHS = 3; // we will shove in and back out 3 times

    const double kSTART_ANGLE = 290.0;
    vector<double> kSTARTING_POS = {0.6, 0.45};

    drive.getOdom().initTracker(kSTARTING_POS[0], kSTARTING_POS[1], kSTART_ANGLE);

    pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::delay(50);
		}
	}}; // lambda function with a task

    // Intake::getInstance()->set_power(127);
    // pros::delay(500);
    // Intake::getInstance()->set_power(0);

    // load until need to go bc of kBOWL_TIME

    pros::Task wings_task{[=] {
        int wing_cycles = 0;
        const double kIN_TIME = 1000;
        const double kHP_WAIT_TIME = 2500;

        while (1) {
            Pneumatics::getInstance()->getWings()->on();
            pros::delay(kHP_WAIT_TIME);
            Pneumatics::getInstance()->getWings()->off();
            wing_cycles++;
            pros::delay(kIN_TIME);
        }
    }};

    // kSKILLS_DURATION - kBOWL_TIME = time we have left to do wings for
    double time_since_start = pros::millis() - starting_time;
    pros::delay((kSKILLS_DURATION - kBOWL_TIME) - (time_since_start));

    // cleanup for wings task
    wings_task.suspend();
    Pneumatics::getInstance()->getWings()->off();

    pros::Task to_bowl_pos_task{[=] {
        
        turnToAngle(drive, 270.0, 3.0);
        vector<vector<double>> path_to_goal = {{drive.getOdom().getX(), drive.getOdom().getY()}, {2.8, 0.3}};

        followPath(path_to_goal, drive, 270, true, false, false, 0.5, 5.0);

        vector<vector<double>> back_up = {path_to_goal.back(), {2.4, 0.3}};
        followPath(back_up, drive, 90, false, true, false, 0.5, 5.0);
    }};


    while (to_bowl_pos_task.get_state() != pros::E_TASK_STATE_DELETED) {
        // while path is not done
        // have this here in case we want to extend/retract wings during path
        pros::delay(100);
    }
    // no need to call suspend because it finishes on its own
   
    vector<vector<double>> push_in = {{drive.getOdom().getX(), drive.getOdom().getY()}, {3.3, 0.3}, {3.3, 0.9}};

    followPath(push_in, drive, 0, false, false, false, 0.5, 4.0, 200.0, 450.0, 150.0);

    for (int i = 0; i < kBACK_FORTHS; i++) {
        vector<vector<double>> go_back = {{drive.getOdom().getX(), drive.getOdom().getY()}, {3.2, 0.6}};
        followPath(go_back, drive, 0, true, true);
        pros::delay(100);
        moveMotors(drive, 150, 150);
        pros::delay(1200);
    }

    moveMotors(drive, -50, -50);
    pros::delay(500);

    Pneumatics::getInstance()->getWings()->off();
    odom_task.suspend();
}