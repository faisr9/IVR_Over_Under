#include "skills_catcher/skills.h"


void push_in() {
    // field centric move hopefully ({1, pi/2}, 0)
    // delay for like 1 sec
    // field centric move opp dir for like 2/3 sec or some tuned amount of time


    // could use pure pursuit/pid later but only if needed
}

LinkHelper* catcher_link = LinkHelper::createInstance(16, E_LINK_TX);

void skills() {

    const double kP = 2.8;

    std::vector<double> start_pos = {0.9, 3.3}; // wrong for now
    const double kSTARTING_ANGLE = 90.0;
    const double kSTART_TIME = pros::millis();
    const double kRECIEVE_FIRST_TIME = 20000; // how long should get triballs at first pos for in millis
    const double kSKILLS_TIME = 45000;

    ast_odom.initTracker(start_pos[0], start_pos[1], 0);
    pros::delay(50);
    pros::Task odom_task{[=] {
        while (1) {
            ast_odom.updatePosition();
            pros::delay(50);
        }
    }};


    pros::Task drive_to_goal_task {[=] {
        std::vector<std::vector<double>> to_goal_path = {start_pos, {2.7, 3.3}, {2.7, 2.3}};
        followPathX(to_goal_path, astdriveCatcher, ast_odom, 90); // todo: update follow path to support diff constants for x drive vs ast drive (or diff robots in general)
    }};

    while (drive_to_goal_task.get_state() != pros::E_TASK_STATE_DELETED) {
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
            drive_to_goal_task.suspend();
            break;
        }

        pros::delay(50);
    }


    // signal to move over to next spot
    pros::Task wait_for_signal {[=] {
        const int kWAIT_TIME = 20000;
        catcher_link->waitForNotify(kWAIT_TIME);
    }};

    double time_since_last_push = 0;
    double last_time = pros::millis();
    const double kPUSH_IN_AFTER = 5000;

    const double kSTART_REC_TIME = pros::millis();

    // while we are still waiting push in every kPUSH_IN_AFTER seconds
    while (wait_for_signal.get_state() != E_TASK_STATE_DELETED) {
        time_since_last_push += pros::millis() - last_time;
        last_time = pros::millis();

        if (time_since_last_push > kPUSH_IN_AFTER) {
            push_in();
            time_since_last_push = 0;
        }


        // FOR TESTING, FOR REAL SKILLS USE VEX LINK
        // if (pros::millis() > kSTART_REC_TIME + kRECIEVE_FIRST_TIME) {
        //     break;
        // }
    }

    const double kLENIENT_PUSH_IN_AFTER = 2000; // something less than kPUSH_IN_AFTER to round up an extra triball
    // time should matter too much maybe bc other robot driving full field?
    if (time_since_last_push > 2000) {
        push_in();
    }

    pros::Task drive_to_second_pos {[=] {
        std::vector<std::vector<double>> to_goal_path = {{ast_odom.getX(), ast_odom.getY()}, {2.7, 1.4}};
        followPathX(to_goal_path, astdriveCatcher, ast_odom, 90);
    }};

    // final thing is don't want to be in the goal when the match ends
    const double kFINAL_PUSH_TIME = 2000; // do final push this many millis before auton end
    time_since_last_push = 0;
    last_time = pros::millis();
    while (pros::millis() < kSTART_TIME + kSKILLS_TIME - kFINAL_PUSH_TIME) {
        time_since_last_push += pros::millis() - last_time;
        last_time = pros::millis();

        if (time_since_last_push > kPUSH_IN_AFTER) {
            push_in();
            time_since_last_push = 0;
        }
    }

    push_in();

    odom_task.suspend();

    // all done!
}