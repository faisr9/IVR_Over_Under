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