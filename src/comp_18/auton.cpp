#include "comp_18/auton.h"

LinkHelper* comp18link = LinkHelper::createInstance(8, E_LINK_RX);

using namespace pros;
using namespace std;

/** Turns the robot to the specified angle.
 * usage example: 
 * 
 *     turn(90);
 *
*/
 void turn(double angle){
	turnToAngle(tank_drive_18, angle, 2.0, false, 0.9); //p=1.12 // turndegtolerance=3 //time 150
 }

void auton18(double auton_duration_time_millis, bool skills) {

	const double kSTART_TIME = pros::millis();

    vector<double> start; //Start position
	vector<vector<double>> curvePath;
	double endY = 0.0;
	// if (!skills) {
		start = {1.342, 0.305};
		endY = 1.6-0.0404;
		curvePath = {start, {2.90-.2, start[1]+0.03}};
		vector<vector<double>> curvePath2 = {curvePath.back(), {2.202+0.043, endY}};
	// } else {
	// 	start = {2.15 * 0.61, 3.3};
	// 	endY = 1.55;
	// 	curvePath = {start, {2.90, 3.2}, {2, endY}};
	// }

	// start = {convert::inToM(54), convert::inToM(12)};
	// curvePath = { 
	// 				start,
	// 			  	{convert::inToM(54+48), convert::inToM(12+3)},
	// 			  	{convert::inToM(54+48-16), convert::inToM(12+3+32)}
	// 			};

	tank_drive_18.getOdom().initTracker(start[0], start[1], 90);
    pros::delay(50);

	pros::Task odom_task{[=] {
		while (1) {
			tank_drive_18.getOdom().updatePosition();
			pros::lcd::set_text(7, "A: " + std::to_string(tank_drive_18.getOdom().getHeading()));
			pros::delay(50);
		}
	}};

    // //1 tile is .61 meters (2 ft)

    // // 1. drop intake (activate climbing piston)
    // Pneumatics::getInstance()->getClimber()->on();

    // // 2. retract climbing piston
    // delay(500);
	// if(skills)
    // 	Pneumatics::getInstance()->getClimber()->off();	

    Intake::getInstance()->set_power(-127 / 1.5);

    // delay(75);

    move(curvePath, 90, false, false);
	Intake::getInstance()->set_power(0);
	turn(350);
	delay(50);

	move(curvePath2, 315, false, false);
	Pneumatics::getInstance()->setLeft(1);
	delay(50);
	turn(89);
	delay(50);
	Pneumatics::getInstance()->toggleWings();

    vector<vector<double>> curvePath3 = {curvePath2.back(), {convert::inToM((24*4)+3.5), endY}};

    // vector<vector<double>> curvePath2 = {curvePath.back(), {2.6, endY}};
    // vector<vector<double>> curvePath4 = {curvePath2.back(), {2.5, endY}};
    // Intake::getInstance()->set_power(127 / 1.5);
    // delay(250);
    // Pneumatics::getInstance()->setLeft(1);
    // followPath(curvePath2, tank_drive_18, 88, false, true, false, 0.5, 3.0, 200.0 / 3.0, 450.0 / 3.0, 30, false, 1.12);
    // //Pneumatics::getInstance()->setLeft(0);
	Intake::getInstance()->set_power(127 / 1.5);
    move(curvePath3, 90, false, false);
	Intake::getInstance()->set_power(0);

	// vector<vector<double>> curvePath4 = {curvePath3.back(), {convert::inToM((24*4)+12), endY}};
	// move(curvePath4, 90, true, false);

	// if(skills)
	// 	Pneumatics::getInstance()->getClimber()->on();
	//  Sid was here
	// waitUntil(comp18link->isLinked());
	// comp18link->waitForNotify(60000);
	// ctrl_master.rumble(".");
	// delay(500);
	// for (int i=0; i<2; i++) {
	// 	ctrl_master.rumble("-");
	// 	delay(1000);
	// }
    
    // //int counter=0;
	// comp18link->notify();

	// // pros::Task endWait{[=] {
	// // 		comp18link->setMsgRecvTimeout(40000);
	// // 		std::string recvMsg = comp18link->recvMsg();
	// // 		if (recvMsg == "end") {
				
	// // 		}
	// // }};

	pros::Task comp18goal_task {[=] {
		bool oncetrigger = false;
		while(1)
		{
			//Intake::getInstance()->set_power(0);
			if (triBall() || !oncetrigger)
			{
				oncetrigger = true;
				Intake::getInstance()->set_power(127);
				delay(250);
				moveMotors(tank_drive_18,60, 60);
				delay(1000);
				left_drive_motors.move_velocity(0);
				right_drive_motors.move_velocity(0);
				double x=tank_drive_18.getOdom().getX(),
					y=tank_drive_18.getOdom().getY();
				vector<vector<double>> oscillate = {{x,y}, {x-.1,y}};
				// vector<vector<double>> oscillate = {{x,y}, {x-(0.025*1),y}};
				//followPath(curvePath2, tank_drive_18, 88, false, true, false, 0.5, 3.0, 200.0 / 3.0, 450.0 / 3.0, 30, false, 1.12);
				move(oscillate, 89, 1, true);
			}
			delay(50);
		}
	}};

	const double kABORT_TIME = kSTART_TIME + auton_duration_time_millis - 500;
	while (pros::millis() < kABORT_TIME) {
        pros::delay(100);
    }
    // comp18goal_task.suspend();
	Pneumatics::getInstance()->getClimber()->off();
    Intake::getInstance()->set_power(0);
}

vector<double> vect(double x, double y){
    return {(x*.61), (y*.61)};
}

vector<double> vectOff(double x, double y){
    return {(.61*2.2)+(x*.61), (.61*.5)+(y*.61)};
}

void move(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
    double speedfactor=3.0;
    followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
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
Start Distance Sensor Loop
---- Cycle #X Times (Recieve Data from Comp 15)
Intake Out
Move Forward (Score Triball)
Move Back
Intake Stop
---- To Cycle
Wait for Comp15 to confirm completion
Drive to contact vertical elevation (Outside)
.END


*/


