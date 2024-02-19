#include "comp_18/auton.h"

LinkHelper* comp18link = LinkHelper::createInstance(8, E_LINK_RX);

using namespace pros;
using namespace std;

vector<double> start; //Start position

void auton18(double auton_duration_time_millis, bool skills) {

	const double kSTART_TIME = pros::millis();

	vector<vector<double>> curvePath;	

	// if (!skills) {
	// 	start = vectOffComp(0, 0);
	// 	endY = 1.6;
	// 	curvePath = {start, {2.90, 0.4}, {2, endY}};
	// } else {
		start = {0.8 * 0.61, 5.1 * 0.61};
		curvePath = {start, vect(1.5,5.5), vect(4,5.5)};
	// }

	tank_drive_18.getOdom().initTracker(start[0], start[1], 45);
    pros::delay(50);

	pros::Task odom_task{[=] {
		while (1) {
			tank_drive_18.getOdom().updatePosition();
			pros::lcd::set_text(7, "A: " + std::to_string(tank_drive_18.getOdom().getHeading()));
			pros::delay(50);
		}
	}};

	vector<vector<double>> curvePath2 = {curvePath.back(), vect(5.5, 4.3)};
	vector<vector<double>> curvePath3Rev = {curvePath2.back(), vect(5.5, 5.5)};
	vector<vector<double>> curvePath3Fwd = {curvePath3Rev.back(), curvePath2.back()};

    //1 tile is .61 meters (2 ft)

	Pneumatics::getInstance()->setLeft(1);
	pros::delay(250);
	turn(75);
	pros::Task comp18bowl_task {[=] {
		for(int i=0; i<9; i++){
			pros::delay(250);
			turn(55);
			pros::delay(1500);
			turn(75);
		}
		Pneumatics::getInstance()->setLeft(0);
		pros::delay(250);
		move(curvePath, 90, false, true);
		pros::delay(250);
		move(curvePath2, 180, false, true);
		pros::delay(250);
		move(curvePath3Rev, 180, true, true);
		pros::delay(250);
		move(curvePath3Fwd, 180, false, true);
		pros::delay(250);
		move(curvePath3Rev, 180, true, true);
		pros::delay(250);
		move(curvePath3Fwd, 180, false, true);
		
	}};

    
    //int counter=0;
	// comp18link->notify();

	// pros::Task endWait{[=] {
	// 		comp18link->setMsgRecvTimeout(40000);
	// 		std::string recvMsg = comp18link->recvMsg();
	// 		if (recvMsg == "end") {
				
	// 		}
	// }};

	const double kABORT_TIME = kSTART_TIME + auton_duration_time_millis - 500;
	while (pros::millis() < kABORT_TIME) {
        pros::delay(100);
    }
    comp18bowl_task.suspend();
	Pneumatics::getInstance()->setLeft(0);
    Intake::getInstance()->set_power(0);
}

/** Converts inputted tile coordinates, x and y, into meters.
 * usage example: 
 * 
 *     vect(1, 2);
 *
*/
vector<double> vect(double x, double y){
    return {(x*.61), (y*.61)};
}

/** Converts inputted tile coordinates, x and y, into meters, with the starting location offset for comp.
 * usage example: 
 * 
 *     vectOffComp(1, 2);
 *
*/
vector<double> vectOff(double x, double y){
    return {(.61*start[0])+(x*.61), (.61*start[1])+(y*.61)};
}

/** Moves the robot to the specified location and angle using movement vectors.
 * usage example: 
 * 
 *     move(vect(1, 2), 90, false, true);
 *
*/
void move(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
    double speedfactor=3.0;
    followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

/** Turns the robot to the specified angle.
 * usage example: 
 * 
 *     turn(90);
 *
*/
 void turn(double angle){
	turnToAngle(tank_drive_18, angle, 3.0, false, 1.12);
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


