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
            pros::delay(100);
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


*/