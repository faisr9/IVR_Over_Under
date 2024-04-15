#include "comp_18/auton.h"

LinkHelper *comp18link = LinkHelper::createInstance(8, E_LINK_RX);

using namespace pros;
using namespace std;

/// COMP AUTON


// A lot of helper functions were removed. Feel free to go back through old commits and re-add them as needed.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//COMP AUTON

void auton18(){
	pros::lcd::set_text(1, "Hiiiiii running auton");
    // (0, 0) corner is where positive x is going to other side of field and positive y is same side of field
    vector<double> start = {0,0}; // Start position
    const double kSTARTING_ANGLE = 90.0;
    const double kAUTON_START_TIME = pros::millis();
    const double kAUTON_DURATION = 45000;

    // const double kAUTON_CODE_DURATION = kAUTON_DURATION - 500;

	tank_drive_18.getOdom().initTracker(start[0], start[1], kSTARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[=] {
		while (1) {
			tank_drive_18.getOdom().updatePosition();
			pros::delay(50);
		}
	}};

	
	move({vect(0,0), vect(48, 0)}, 90, false, false);
	pros::delay(50);
	move({vect(48, 0), vect(0, 48)}, 90, true, false);
	pros::delay(2000);
	odom_task.suspend();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void move(vector<vector<double>> vec, int angle, bool isReversed, bool isSpinAtEnd, double speedfactor){
	followPath(vec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 600 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

void turn(double angle){
	turnToAngle(tank_drive_18, angle, 0.5, false, 1.9);
}

//Converts from inches to meters
vector<double> vect(double x, double y){
	return {x * 0.0254, y * 0.0254};
}

