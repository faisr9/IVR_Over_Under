#include "comp_15/auton.h"

LinkHelper *comp15link = LinkHelper::createInstance(8, E_LINK_RX);

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
	turnToAngle(tank_drive_15, angle, 2.0, false, 1); // p=1.12 // turndegtolerance=3 //time 150
}

void move_slw_comp(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
	double speedfactor = 3.87;
	followPath(moveVec, tank_drive_15, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

vector<double> vectOff_comp(double x, double y)
{
	return {(.61 * 2.2) + (x * .61), (.61 * .5) + (y * .61)};
}

void move_comp(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
	double speedfactor = 2.7;
	followPath(moveVec, tank_drive_15, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//COMP AUTON

void auton15(double auton_duration_time_millis, bool skills){

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SKILLS AUTON

vector<double> skills_start = {1.2 * 0.61, 5.4 * 0.61};
 // Start position

void skills15(bool driver){

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
	followPath(moveVec, tank_drive_15, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 250.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, .91);
}

/** Turns the robot to the specified angle.
 * usage example:
 *
 *     turn(90);
 *
 */
void turn_skills(double angle)
{
	turnToAngle(tank_drive_15, angle, 3.0, false, .91, 150); // p=1.12 // turndegtolerance=3 //time 150
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
	turnToAngle(tank_drive_15, angle, 10.0, false, 1.5, 50); // p=1.12 // turndegtolerance=3 //time 150
}
