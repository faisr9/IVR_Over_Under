#include "comp_18/auton.h"

LinkHelper *comp18link = LinkHelper::createInstance(8, E_LINK_RX);

using namespace pros;
using namespace std;

/// COMP AUTON

/// COMP METHODS

/** Converts inputted tile coordinates, x and y, into meters.
 * usage example:
 *
 *     vect(1, 2);
 *
 */
vector<double> vect(double x, double y)
{
	return {(x * .61), (y * .61)};
}

void turn_comp(double angle)
{
	turnToAngle(tank_drive_18, angle, 2.0, false, 1); // p=1.12 // turndegtolerance=3 //time 150
}

void move_slw_comp(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
	double speedfactor = 3.87;
	followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

vector<double> vectOff_comp(double x, double y)
{
	return {(.61 * 2.2) + (x * .61), (.61 * .5) + (y * .61)};
}

void move_comp(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
	double speedfactor = 2.7;
	followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

void auton18(double auton_duration_time_millis, bool skills)
{

	const double kSTART_TIME = pros::millis();

	vector<double> start; // Start position
	vector<vector<double>> curvePath;
	double endY = 0.0;
	start = {1.342, 0.305};
	// endY = 1.6-0.0404-convert::inToM(1.5);
	endY = 1.562 + convert::inToM(2);
	curvePath = {start, {2.90 - .2, start[1] + 0.03}};
	vector<vector<double>> curvePath2 = {curvePath.back(), {2.202 + 0.043, endY}};

	tank_drive_18.getOdom().initTracker(start[0], start[1], 90);
	pros::delay(50);

	pros::Task odom_task{[=]
						 {
							 while (1)
							 {
								 tank_drive_18.getOdom().updatePosition();
								 pros::lcd::set_text(7, "A: " + std::to_string(tank_drive_18.getOdom().getHeading()));
								 pros::delay(50);
							 }
						 }};

	// Turn on Intake
	Intake::getInstance()->set_power(-127);

	// Move to other side
	move_comp(curvePath, 90, false, false);
	Intake::getInstance()->set_power(0);
	turn_comp(335);
	delay(50);

	// Move to center of field
	move_comp(curvePath2, 315, false, false);
	Pneumatics::getInstance()->setLeft(1);
	delay(50);
	turn_comp(89);
	delay(50);
	Pneumatics::getInstance()->toggleWings();

	// vector<vector<double>> curvePath3 = {curvePath2.back(), {convert::inToM((24*4)+3.5), endY}};
	vector<vector<double>> curvePath3 = {curvePath2.back(), {2.654 - convert::inToM(3.5), endY}};
	// Pneumatics::getInstance()->getClimber()->toggle();
	Intake::getInstance()->set_power(127 / 1.5);
	move_comp(curvePath3, 89, false, false);
	// Intake::getInstance()->set_power(0);

	// 1st end: 2.619x 1.567y
	// touching goal, 2.654x, 1.566y
	// lined up back, 2.578x, 1.566y
	// iside goal: 2.85x, 1.566y

	// touching - lined up back = 0.076x, 0y

	// while(1)
	// {
	// 	pros::delay(5000);
	// }
	// double x=tank_drive_18.getOdom().getX(),
	// 	y=tank_drive_18.getOdom().getY();

	vector<double> atGoal = {tank_drive_18.getOdom().getX(), tank_drive_18.getOdom().getY()};
	vector<double> insideGoal = {2.80, 1.566};
	vector<double> linedUpBack = {2.578, 1.566};

	// 2.43x, 1.556y

	pros::Task comp18goal_task{[=]
	{
		bool oncetrigger = false;
		while (1)
		{
			// Intake::getInstance()->set_power(0);
			if (triBall() || !oncetrigger)
			{
				// if(oncetrigger)
				delay(500); // Allow triball to be removed from intake
				// Intake::getInstance()->set_power(127);
				// moveMotors(tank_drive_18, 85, 85);
				// delay(900); // Drive forward delay
				// left_drive_motors.move_velocity(0);
				// right_drive_motors.move_velocity(0);
				// vector<vector<double>> oscillate = {{x,y}, {x-.07,y}};
				// move(oscillate, 89, true, true);
				// delay(50);

				Intake::getInstance()->set_power(127);
				// vector<vector<double>> entergoal_path = {{atGoal[0], atGoal[1]}, {insideGoal[0], insideGoal[1]}};
				// move_comp(entergoal_path, 89, false, false);
				moveMotors(tank_drive_18, 80, 80);
				delay(1500);
				stopMotors(tank_drive_18);

				vector<vector<double>> exitgoal_path = {{insideGoal[0], insideGoal[1]},
														{linedUpBack[0], linedUpBack[1]}};
				// {x-convert::inToM(10), linedUpBack[1]}};
				move_slw_comp(exitgoal_path, 89, true, false);
				delay(50);

				// double x = tank_drive_18.getOdom().getX(),
				// 		y = tank_drive_18.getOdom().getY();

				// if (x < linedUpBack[0] - convert::inToM(2.5))
				// {
				// 	vector<vector<double>> oscillate = {{x,y}, {linedUpBack[0],y}};
				// 	move_slw_comp(oscillate, 89, false, false);
				// 	delay(50);
				// }

				if (!oncetrigger)
				{
					Pneumatics::getInstance()->stop();
					comp18link->notify();
					oncetrigger = true;
				}
			}
			delay(50);
		}
	}};

	const double kABORT_TIME = kSTART_TIME + auton_duration_time_millis - 500;
	while (pros::millis() < kABORT_TIME)
	{
		pros::delay(70);
	}
	comp18goal_task.suspend();
	Pneumatics::getInstance()->stop();
	Intake::getInstance()->stop();
	odom_task.suspend();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SKILLS AUTON

vector<double> skills_start = {1.2 * 0.61, 5.5 * 0.61};
 // Start position

void skills18(bool driver)
{

	const double kSTART_TIME = pros::millis();

	tank_drive_18.getOdom().initTracker(skills_start[0], skills_start[1], 225);
	pros::delay(50);

	pros::Task odom_task{[=]
	{
		while (1)
		{
			tank_drive_18.getOdom().updatePosition();
			pros::lcd::set_text(7, "A: " + std::to_string(tank_drive_18.getOdom().getHeading()));
			pros::delay(50);
		}
	}};

	// 1 tile is .61 meters (2 ft)

	while (tank_drive_18.getOdom().getX() > 1.15 * .61)
	{ // 2*M_PI
		moveMotors(tank_drive_18, 20, 20);
		pros::delay(50);
	}
	stopMotors(tank_drive_18);

	Pneumatics::getInstance()->setRight(1);

	pros::delay(250);
	turnF_skills(270);
	pros::delay(250);

	pros::Task bowl_task{[=]
		{
		int iter;
		if(driver)
			iter = 22;
		else 
			iter = 14;
		for (int i = 0; i < iter; i++)
		{
			// Pneumatics::getInstance()->setRight(0);
			pros::delay(250 / 1.5);
			turnF_skills(230);
			pros::delay(500 / 2);
			// Pneumatics::getInstance()->setRight(1);
			turnF_skills(270);
		}
	}};

	while (bowl_task.get_state() != pros::E_TASK_STATE_DELETED)
	{
		pros::delay(50);
		if(driver&&ctrl_master.get_digital(BUTTON_B))
		{
			bowl_task.suspend();
			odom_task.suspend();
			stopMotors(tank_drive_18);
			return;
		}
	}
	stopMotors(tank_drive_18);
	if(driver)
	{
		odom_task.suspend();
		return;
	}
	pros::delay(150);

	turn_skills(225);
	pros::delay(150);

	while (tank_drive_18.getOdom().getX() > (.95) * .61)
	{ // 2*M_PI
		moveMotors(tank_drive_18, 50, 50);
		pros::delay(50);
	}
	stopMotors(tank_drive_18);
	Pneumatics::getInstance()->setRight(0);

	pros::delay(150);
	turn_skills(45);
	pros::delay(150);
	Pneumatics::getInstance()->setRight(1);

	vector<vector<double>> curvePath1 = {{tank_drive_18.getOdom().getX(), tank_drive_18.getOdom().getY()}, vect(1.3, 5.6), vect(1.5, 5.66), vect(4, 5.66), vect(4.77, 5.66), vect(5.2,5.2)};

	pros::Task acrossMid_task{[=]
		{
			move_skills(curvePath1, 135, false, true, 2.7);
		}};

	while (acrossMid_task.get_state() != pros::E_TASK_STATE_DELETED)
	{
		Intake::getInstance()->set_power(127);
		if (tank_drive_18.getOdom().getX() < 3.1 * .61 && tank_drive_18.getOdom().getX() > 2.2 * .61)
			Pneumatics::getInstance()->setRight(0);
		else
			Pneumatics::getInstance()->setRight(1);
		pros::delay(50);
	}

	turn_skills(135);

	// vect(5,5.4),vect(5.2, 4.5),
	//  vector<vector<double>> push_in = {{tank_drive_18.getOdom().getX(),tank_drive_18.getOdom().getX(), vect(1.15, 5.62), vect(4.9, 5.62), vect(5.55, 5.82), vect(5.41, 4.48)};

	///////////// ---
	// pros::delay(150);

	// pros::Task toGoal_task{[=]
	// 	{
	// 		// move(curvePath2, 160, false, true, 2);
	// 		vector<vector<double>> curvePath2 = {curvePath1.back(), vect(5.2, 4.9)};
	// 		followPath(curvePath2, tank_drive_18, 160, false, true, false, 0.5, 3.0, 250.0 / 1.8, 450.0 / 2.5, 40.0 / 2.5, false, .91);
	// 	}};

	// while (toGoal_task.get_state() != pros::E_TASK_STATE_DELETED)
	// {
	// 	Pneumatics::getInstance()->setRight(1);
	// 	if (tank_drive_18.getOdom().getX() < 4.8 * .61 && tank_drive_18.getOdom().getY() > 5.1 * .61)
	// 		Pneumatics::getInstance()->setLeft(1);
	// 	else
	// 		Pneumatics::getInstance()->setLeft(0);
	// 	pros::delay(50);
	// }
	//////////////// ----

	pros::delay(150);
	// vector<vector<double>> curvePath3Rev = {curvePath2.back(), vect(5.3, 5.5)};
	// vector<vector<double>> curvePath3Fwd = {curvePath3Rev.back(), curvePath2.back()};

	pros::Task goBack_task{[=]
		{
			for (int i = 0; i < 5; i++)
			{
				Intake::getInstance()->set_power(127);
				moveMotors(tank_drive_18, 300, 300);
				pros::delay(750);
				stopMotors(tank_drive_18);
				pros::delay(200);
				turn_skills(180);
				// pure pursuit not working
				// vector<vector<double>> go_back = {{tank_drive_18.getOdom().getX(), tank_drive_18.getOdom().getY()}, vect(5.2, 4.8)};
				// move_skills(go_back, 165, true, true, 2.7);
				moveMotors(tank_drive_18, -50, -50);
				pros::delay(400);
				stopMotors(tank_drive_18);
				pros::delay(200);
				if(i%3==0){
					turnF_skills(145);
				} else 
					turnF_skills(180);
				pros::delay(150);
			}
		}};

	int auton_duration_time_millis = 60000;
	const double kABORT_TIME = kSTART_TIME + auton_duration_time_millis - 500;
	while (pros::millis() < kABORT_TIME)
	{
		Pneumatics::getInstance()->setRight(1);
		if (tank_drive_18.getOdom().getX() < 4.8 * .61 && tank_drive_18.getOdom().getY() > 5.1 * .61)
			Pneumatics::getInstance()->setLeft(1);
		else
			Pneumatics::getInstance()->setLeft(0);
		pros::delay(100);
	}
	Pneumatics::getInstance()->setLeft(0);
	Pneumatics::getInstance()->setRight(0);
	Intake::getInstance()->set_power(0);
	odom_task.suspend();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SKILLS METHODS

/** Converts inputted tile coordinates, x and y, into meters, with the starting location offset for comp.
 * usage example:
 *
 *     vectOff_skills(1, 2);
 *
 */
vector<double> vectOff_skills(double x, double y)
{
	return {(.61 * skills_start[0]) + (x * .61), (.61 * skills_start[1]) + (y * .61)};
}

/** Moves the robot to the specified location and angle using movement vectors.
 * usage example:
 *
 *     move(vect(1, 2), 90, false, true);
 *
 */
void move_skills(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor)
{
	followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 250.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, .91);
}

/** Turns the robot to the specified angle.
 * usage example:
 *
 *     turn(90);
 *
 */
void turn_skills(double angle)
{
	turnToAngle(tank_drive_18, angle, 3.0, false, .915, 150); // p=1.12 // turndegtolerance=3 //time 150
}
/*

/** Turns the robot to the specified angle with increased velocity.
 * usage example:
 *
 *     turn(90);
 *
*/
void turnF_skills(double angle)
{
	turnToAngle(tank_drive_18, angle, 10.0, false, 1.5, 50); // p=1.12 // turndegtolerance=3 //time 150
}
