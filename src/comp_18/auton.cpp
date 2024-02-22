#include "comp_18/auton.h"

LinkHelper* comp18link = LinkHelper::createInstance(8, E_LINK_RX);

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
	start = {1.342, 0.305};
	endY = 1.6-0.0404-convert::inToM(1.5);
	curvePath = {start, {2.90 - .2, start[1] + 0.03}};
	vector<vector<double>> curvePath2 = {curvePath.back(), {2.202 + 0.043, endY}};

	tank_drive_18.getOdom().initTracker(start[0], start[1], 90);
    pros::delay(50);

	pros::Task odom_task{[=] {
		while (1) {
			tank_drive_18.getOdom().updatePosition();
			pros::lcd::set_text(7, "A: " + std::to_string(tank_drive_18.getOdom().getHeading()));
			pros::delay(50);
		}
	}};

	// Turn on Intake
    Intake::getInstance()->set_power(-127);

	// Move to other side
    move(curvePath, 90, false, false);
	Intake::getInstance()->set_power(0);
	turn(335);
	delay(50);

	// Move to center of field
	move(curvePath2, 315, false, false);
	Pneumatics::getInstance()->setLeft(1);
	delay(50);
	turn(89);
	delay(50);
	Pneumatics::getInstance()->toggleWings();

    vector<vector<double>> curvePath3 = {curvePath2.back(), {convert::inToM((24*4)+3.5), endY}};

    move(curvePath3, 90, false, false);
	Intake::getInstance()->set_power(127 / 1.5);
	// Intake::getInstance()->set_power(0);

	// Sid was here
	
	pros::Task comp18goal_task {[=] {
		bool oncetrigger = false;
		while(1)
		{
			//Intake::getInstance()->set_power(0);
			if (triBall() || !oncetrigger)
			{
				if(oncetrigger)
					delay(180); // Allow triball to be removed from intake
				Intake::getInstance()->set_power(127);
				moveMotors(tank_drive_18, 85, 85);
				delay(850); // Drive forward delay
				left_drive_motors.move_velocity(0);
				right_drive_motors.move_velocity(0);
				double x=tank_drive_18.getOdom().getX(),
					y=tank_drive_18.getOdom().getY();
				vector<vector<double>> oscillate = {{x,y}, {x-.065,y}};
				move(oscillate, 89, true, true);
				delay(50);

				if (!oncetrigger)
				{
					Pneumatics::getInstance()->setRight(0);
					comp18link->notify();
				}

				oncetrigger = true;
			}
			delay(50);
		}
	}};

	const double kABORT_TIME = kSTART_TIME + auton_duration_time_millis - 500;
	while (pros::millis() < kABORT_TIME) {
        pros::delay(70);
    }
    comp18goal_task.suspend();
	Pneumatics::getInstance()->stop();
    Intake::getInstance()->stop();
	odom_task.suspend();
}

vector<double> vect(double x, double y){
    return {(x*.61), (y*.61)};
}

vector<double> vectOff(double x, double y){
    return {(.61*2.2)+(x*.61), (.61*.5)+(y*.61)};
}

void move(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
    double speedfactor=2.7;
    followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}



/*


15-Comp
=====================
Intake to Open Bot
Cata Down
---- Cycle Up to 10 Times; Dynamically stops via time
Doinker Down (Once Cata is down)
Doinker Up
~ Wait for Comp 18 (Get to Position | Cycle Triball into goal)
	~ Set timeout delay
Cata Cycle
Increment Counter on Controller
Delay 2 sec (For placing Tribal)
---- To Cycle
Cata Release
Doinker Up
Drive to Contact Vertial Elevation (IF AWP)
.END

18-Comp
=====================
Intake Down
Retract Climb Pistion
Drive to Goal
Notify Comp15 (At goal ready for cycle)
Start Distance Sensor Loop
---- Cycle til auton end
Intake Out
Move Forward (Score Triball)
Move Back
Intake Stop
---- To Cycle
Drive to contact vertical elevation (IF AWP)
.END


*/


