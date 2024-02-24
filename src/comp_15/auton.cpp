#include "comp_15/auton.h"

LinkHelper* comp15link = LinkHelper::createInstance(16, E_LINK_TX);

void auton_15(double auton_duration_time_millis, bool climb) {

    pros::lcd::set_text(1, "Hiiiiii running auton");
    // (0, 0) corner is where positive x is going to other side of field and positive y is same side of field
    const double kSTARTING_X = 0.5;
    const double kSTARTING_Y = 0.3;
    const double kSTARTING_ANGLE = 60.0;

    const double kAUTON_START_TIME = pros::millis();
    // const double kAUTON_CODE_DURATION = auton_duration_time_millis - 500;

    drive.getOdom().initTracker(kSTARTING_X, kSTARTING_Y, kSTARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::delay(50);
		}
	}}; // lambda function with a task

    // Use kickstand to prevent shift when opening bot
    Pneumatics::getInstance()->getFloorBrake()->on();
    pros::delay(250);
    // release intake and catapult
    Intake::getInstance()->set_power(-127);
    pros::delay(250);
    Intake::getInstance()->set_power(0);
    delay(1500); // Allow robot to settle
    // release kickstand
    Pneumatics::getInstance()->getFloorBrake()->off();
    Pneumatics::getInstance()->stop();

    // start shooting portion of autonomous
    pros::Task shooting_task{[=] {
        bool oncetrigger = false;
        CompetitionCatapult::getInstance()->set_cata_mode("P");
        // Ensure that the catapult is primed before starting the shooting task
        while(!CompetitionCatapult::getInstance()->get_switch_state()){
            CompetitionCatapult::getInstance()->set_cata_mode("P");
            pros::lcd::set_text(2, "Priming");
        }
        pros::delay(2500); // longer delay for two triballs

	    int cycleCounter = 0;
        while (1) {
            // doinker down
            DoinkerClass::getInstance()->move(DoinkerClass::DOWN); // did not go past here
            // small delay
            pros::delay(400);
            // doinker up if cata primed
            DoinkerClass::getInstance()->move(DoinkerClass::UP);
            // medium delay
            pros::delay(750);

            if (!oncetrigger) {
                comp15link->waitForNotify(10000);
                oncetrigger = true;
            }
            // else
                // comp15link->waitForNotify(1500);
            
            // fire catapult and put it back down
            CompetitionCatapult::getInstance()->set_cata_mode("RP");
            delay(50);
            while(!CompetitionCatapult::getInstance()->get_switch_state()){
                    CompetitionCatapult::getInstance()->set_cata_mode("P");
                    pros::lcd::set_text(2, "Priming");
                }
            cycleCounter++;
            // wait for human player
            pros::delay(2100);
        }
	}}; // lambda function with a task


    // create while loop that will exit after kMAX_SHOOTING_TIME/1000 seconds
    // const int kMAX_SHOOTING_TIME = 30000;
    // const double start_shoot_time = pros::millis();
    // create while loop that will exit right before the end of auton
    // const int kMAX_SHOOTING_TIME = 30000;
    // const double start_shoot_time = pros::millis();
    // no need to check if task has terminated because it can't
    bool set_rumble = false;

    // const double kSHOOT_ABORT_TIME = (climb) ? kAUTON_START_TIME + kAUTON_CODE_DURATION - kCLIMB_TIME : kAUTON_START_TIME + kAUTON_CODE_DURATION;
    // while (pros::millis() < kSHOOT_ABORT_TIME) {
    //     if (pros::millis() > kSHOOT_ABORT_TIME - 2000 && !set_rumble) {
    //         ctrl_master.rumble("--");
    //         set_rumble = true;
    //     }
    //     pros::delay(100);
    // }
    const double kABORT_TIME = kAUTON_START_TIME + auton_duration_time_millis - 500;
	while (pros::millis() < kABORT_TIME) {
        if(pros::millis() > kABORT_TIME - 1700 && !set_rumble){
            ctrl_master.rumble("--");
            set_rumble = true;
        }
        pros::delay(70);
    }

	if(CompetitionCatapult::getInstance()->get_switch_state())
		CompetitionCatapult::getInstance()->set_cata_mode("R");
    
    shooting_task.suspend();
    odom_task.suspend();
}

/*


15-Comp
=====================
$ Brakes Down
$ Intake to Open Bot
$ Cata Down
---- Cycle #X Times (Send this number via string to Comp18)
Doinker Down (Once Cata is down)
Doinker Up
~ Wait for Comp 18 (Get to Position | Cycle Triball into goal)
	~ Set timeout delay
Cata Cycle
Increment Counter on Controller
Controller Rumble?
Delay 1.5 sec (For placing Tribal)
---- To Cycle
Cata Release
Doinker Up
Notify Comp18
Drive to Contact Vertial Elevation (Inside)
.END

18-Comp
=====================
Intake Down
Retract Climb Pistion
Drive to Goal (AWP Attenpt)
Notify Comp15 (At goal ready for cycle)
---- Cycle #X Times (Recieve Data from Comp 15)
Notify Comp15
~ Wait Til Distance Trigger
	~ Set timeout
Intake Out
Move Forward (Score Triball)
Move Back
Intake Stop
---- To Cycle
Wait for Comp15 to confirm completion
Drive to contact vertical elevation (Outside)
.END

Load Bar Shift Right
Back right contact load bar
align to white tape


*/


void skills_15(bool driver) {
    const double starting_time = pros::millis();

    const double kSKILLS_DURATION = (driver) ? 55000 : 45000; // NEED TO CHANGE THIS LATER, 40000 FOR TESTING

    const double kBOWL_TIME = 25000; // time we are driving the robot and pushing things in for
    const double kBACK_FORTHS = 2; // we will shove in and back out 3 times

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
    turnToAngle(drive, 120, 3.0, false, 1.9, 150);

    moveMotors(drive, -150, -150);
    pros::delay(150);
    stopMotors(drive);

    pros::Task to_goal_task {[=] {
        Intake::getInstance()->set_power(-127);
        
        vector<vector<double>> push_in = {{drive.getOdom().getX(), drive.getOdom().getY()}, {0.7, 0.38}, {3.0, 0.38}, {3.4, 0.5}, {3.3, 0.9}}; // was 2.5 and 3.0 earlier!!!

        followPath(push_in, drive, 0, false, false, false, 0.5, 4.0, 150.0, 450.0, 100.0, false, 1.9);
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
    for (int i = 0; i < kBACK_FORTHS; i++) {
        vector<vector<double>> go_back = {{drive.getOdom().getX(), drive.getOdom().getY()}, {3.3, 0.7}};
        followPath(go_back, drive, 0, true, true);
        pros::delay(100);
        moveMotors(drive, 140, 140);

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