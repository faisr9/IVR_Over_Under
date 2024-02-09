#include "comp_15/auton.h"


void auton_15() {

    pros::lcd::set_text(1, "Hiiiiii running auton");
    // (0, 0) corner is where positive x is going to other side of field and positive y is same side of field
    const double kSTARTING_X = 0.5;
    const double kSTARTING_Y = 0.3;
    const double kSTARTING_ANGLE = 60.0;

    drive.getOdom().initTracker(kSTARTING_X, kSTARTING_Y, kSTARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::delay(50);
		}
	}}; // lambda function with a task

    // release intake and catapult
    Intake::getInstance()->set_power(50);
    pros::delay(500);
    Intake::getInstance()->set_power(0);

    // start shooting portion of autonomous
    pros::Task shooting_task{[=] {
        while (1) {
            CompetitionCatapult::getInstance()->set_cata_mode("P");
            // cata down
            // delay for triball placement
            pros::delay(3000);
            // doinker down
            DoinkerClass::getInstance()->move(DoinkerClass::DOWN);
            // small delay
            pros::delay(500);
            // doinker up
            DoinkerClass::getInstance()->move(DoinkerClass::UP);
            // medium delay
            pros::delay(2000);
            // cata up
            CompetitionCatapult::getInstance()->set_cata_mode("RP");
            // repeat

            pros::delay(1000);
        }
	}}; // lambda function with a task

    // create while loop that will exit after kMAX_SHOOTING_TIME/1000 seconds
    const int kMAX_SHOOTING_TIME = 15000;
    const double start_shoot_time = pros::millis();
    // no need to check if task has terminated because it can't
    while (pros::millis() < start_shoot_time + kMAX_SHOOTING_TIME) {
        pros::delay(100);
    }
    shooting_task.suspend();
    // reset catapult and doinker to known state
    CompetitionCatapult::getInstance()->set_cata_mode("R");
    DoinkerClass::getInstance()->move(DoinkerClass::UP);


    // drive robot to pole to touch
    pros::Task path_task{[=] {
        std::vector<std::vector<double>> to_pole = {{kSTARTING_X, kSTARTING_Y}, {1.0, 1.2}, {1.58, 0.66}};
        followPath(to_pole, drive, 135, false, false, false, 0.5, 3.0, 200.0, 450.0, 200.0);
	}}; // lambda function with a task

    // pros::lcd::set_text(7, "During path state" + std::to_string(path_task.get_state()));

    // another abort bc the robot might not get precisely to the point (and just be stuck driving into the pole)
    const double kPATH_MAX_TIME = 3200;
    double start_time = pros::millis();
    while (pros::millis() < start_time + kPATH_MAX_TIME && path_task.get_state() != pros::E_TASK_STATE_DELETED) {
        pros::delay(100);
    }

    // if the path is still going cancel it, but if it's finished don't call suspend
    if (path_task.get_state() != pros::E_TASK_STATE_DELETED && path_task.get_state() != pros::E_TASK_STATE_INVALID) {
        // if call suspend on deleted task program will crash! (data abort)
        // this is why program would only crash if program successfully completed
        path_task.suspend();
        stopMotors(drive);
    }

    // pros::E_TASK_STATE enums go chronologically based on order listed in docs starting at 0
    // OK SO WHAT HAPPENS IS IF THE TASK IS DONE PROS AUTOMATICALLY DELETES IT SO CALLING SUSPEND ON DELETED TASK = CRASH
    // CAN TELL WITH GET_STATE == E_TASK_STATE_DELETED

	odom_task.suspend();
    pros::lcd::set_text(1, "Done with the path!");
}

void skills_15() {

    pros::lcd::set_text(1, "Hiiiiii running auton");
    // (0, 0) corner is where positive x is going to other side of field and positive y is same side of field
    const double kSTARTING_X = 0.5;
    const double kSTARTING_Y = 0.3;
    const double kSTARTING_ANGLE = 60.0;

    drive.getOdom().initTracker(kSTARTING_X, kSTARTING_Y, kSTARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::delay(50);
		}
	}}; // lambda function with a task


    Intake::getInstance()->set_power(50);
    pros::delay(500);

    Intake::getInstance()->set_power(0);

    
    pros::Task path_task{[=] {
        std::vector<std::vector<double>> to_climb_bar = {{kSTARTING_X, kSTARTING_Y}, {2.4, 0.2}};
        followPath(to_climb_bar, drive, 90, false, false, false, 0.5, 3.0, 220.0, 450.0, 220);
	}}; // lambda function with a task

    const double kPATH_MAX_TIME = 2800;
    double start_time = pros::millis();
    while (pros::millis() < start_time + kPATH_MAX_TIME && path_task.get_state() != pros::E_TASK_STATE_DELETED) {
        pros::delay(100);
    }

    // if the path is still going
    if (path_task.get_state() != pros::E_TASK_STATE_DELETED && path_task.get_state() != pros::E_TASK_STATE_INVALID) {
        // if call suspend on deleted task program will crash! (data abort)
        // this is why program would only crash if program successfully completed
        path_task.suspend();
        stopMotors(drive);
    }

    CompetitionCatapult::getInstance()->set_cata_mode("P");

    pros::delay(250);

    CompetitionCatapult::getInstance()->set_cata_mode("R");

    odom_task.suspend();
}