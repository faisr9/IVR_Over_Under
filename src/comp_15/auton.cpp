#include "comp_15/auton.h"

LinkHelper* comp15link = LinkHelper::createInstance(16, E_LINK_TX);

void auton_15(bool climb) {

    pros::lcd::set_text(1, "Hiiiiii running auton");
    // (0, 0) corner is where positive x is going to other side of field and positive y is same side of field
    const double kSTARTING_X = 0.5;
    const double kSTARTING_Y = 0.3;
    const double kSTARTING_ANGLE = 60.0;
    const double kAUTON_START_TIME = pros::millis();
    const double kAUTON_DURATION = 45000;

    // const double kAUTON_CODE_DURATION = kAUTON_DURATION - 500;

    drive.getOdom().initTracker(kSTARTING_X, kSTARTING_Y, kSTARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::delay(50);
		}
	}};

    // All the old auton stuff has been removed because we'll have brand new robots. 
    // Remember you can look back at old versions of master on our github (like the Release
    // for Purdue) to see the auton structures that were once here!

    odom_task.suspend();
}
