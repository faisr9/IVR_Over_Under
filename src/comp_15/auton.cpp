#include "comp_15/auton.h"

LinkHelper* comp15link = LinkHelper::createInstance(16, E_LINK_TX);

//COMP 15 AUTON
void auton_15() {

    pros::lcd::set_text(1, "Hiiiiii running auton");
    // (0, 0) corner is where positive x is going to other side of field and positive y is same side of field
    const double kSTARTING_X = 0.5;
    const double kSTARTING_Y = 0.3;
    const double kSTARTING_ANGLE = 60.0;
    const double kAUTON_START_TIME = pros::millis();
    const double kAUTON_DURATION = 45000;

    // const double kAUTON_CODE_DURATION = kAUTON_DURATION - 500;

    tank_drive_15.getOdom().initTracker(kSTARTING_X, kSTARTING_Y, kSTARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[=] {
		while (1) {
			tank_drive_15.getOdom().updatePosition();
			pros::delay(50);
		}
	}};

    // All the old auton stuff has been removed because we'll have brand new robots. 
    // Remember you can look back at old versions of master on our github (like the Release
    // for Purdue) to see the auton structures that were once here!

    odom_task.suspend();
}

// COMP METHODS

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