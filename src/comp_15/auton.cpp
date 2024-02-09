#include "comp_15/auton.h"


void auton_15() {

    pros::lcd::set_text(1, "Hiiiiii running auton");
    // (0, 0) corner is where positive x is going to other side of field and positive y is same side of field
    const double kSTARTING_X = 0.5;
    const double kSTARTING_Y = 0.3;
    const double kSTARTING_ANGLE = 60.0;


    pros::Task shooting_task{[=] {

        while (1) {
            pros::delay(1000);
        }

        // followPath(to_pole, drive, 135, false, false, false, 0.5, 3.0, 200.0, 450.0, 200.0);
	}}; // lambda function with a task

    const int kMAX_SHOOTING_TIME = 15000;
    double start_shoot_time = pros::millis();
    while (pros::millis() < start_shoot_time + kMAX_SHOOTING_TIME && path_task.get_state() != pros::E_TASK_STATE_DELETED) {
        pros::delay(100);
    }

    drive.getOdom().initTracker(kSTARTING_X, kSTARTING_Y, kSTARTING_ANGLE);
    pros::delay(50);

    pros::Task odom_task{[=] {
		while (1) {
			drive.getOdom().updatePosition();
			pros::delay(50);
		}
	}}; // lambda function with a task

	// do something noteworthy



    Intake::getInstance()->set_power(50);
    pros::delay(500);

    Intake::getInstance()->set_power(0);

    
    pros::Task path_task{[=] {
        std::vector<std::vector<double>> to_pole = {{kSTARTING_X, kSTARTING_Y}, {1.0, 1.2}, {1.58, 0.66}};
        followPath(to_pole, drive, 135, false, false, false, 0.5, 3.0, 200.0, 450.0, 200.0);
	}}; // lambda function with a task

    // pros::lcd::set_text(7, "During path state" + std::to_string(path_task.get_state()));

    const double kPATH_MAX_TIME = 3200;
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

    // ready = 1
    // running = 0
    // suspended = 3
    // goes chronlogical

    // crashing if suspending after path successfully finishes

    // pros::lcd::set_text(7, "After path state" + std::to_string(path_task.get_state()));
    // path_task.remove();

    // uint32_t state = path_task.get_state();
    // if (state == pros::E_TASK_STATE_RUNNING) {
    //     pros::lcd::set_text(7, "Path state is running");
    // } else if (state == pros::E_TASK_STATE_SUSPENDED) {
    //     pros::lcd::set_text(7, "Path state is suspended");
    // } else {
    //     pros::lcd::set_text(7, "Path state is something else");
    // }
    // pros::lcd::set_text(6, std::to_string(state));
    // pros::delay(100);

    // OK SO WHAT HAPPENS IS IF THE TASK IS DONE PROS AUTOMATICALLY DELETES IT SO CALLING SUSPEND ON DELETED TASK = CRASH
    // CAN TELL WITH GET_STATE == E_TASK_STATE_DELETED




    pros::lcd::set_text(1, "Starting the path!");

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