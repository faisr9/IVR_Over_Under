#include "skills_catcher/skills.h"


void push_in() {
    catcher_wings.off();
    turnToAngleX(astdriveCatcher, ast_odom, 90, 3.0);
    catcher_wings.on();

    astdriveCatcher.robot_centric_move({0.5, 0}, 0);
    pros::delay(1000);
    std::vector<std::vector<double>> back_path = {{ast_odom.getX(), ast_odom.getY()}, {2.5, 2.1}};
    catcher_follow_path(back_path, 180);
    catcher_wings.on();
    // field centric move hopefully ({1, pi/2}, 0)
    // delay for like 1 sec
    // field centric move opp dir for like 2/3 sec or some tuned amount of time


    // could use pure pursuit/pid later but only if needed
    pros::delay(100);
}

// LinkHelper* catcher_link = LinkHelper::createInstance(99, E_LINK_TX);
const double kTURN_P = 2.7;

void catcher_follow_path(std::vector<std::vector<double>>& path, double final_angle) {
    followPathX(path, astdriveCatcher, ast_odom, final_angle, true, false, 0.2, 3.0, 200.0, 200.0, 20.0, false, kTURN_P);
}


void skills() {

    /*
    MANY X-DRIVE FUNCTIONS DO NOT HAVE PROPER ASTERISK DRIVE ACCOMODATIONS
    EXAMPLES:
        TurnToAngleX (final turn at end of path failing) 
        turn_with_power()
        some of the field/robot centric moves (and some of these are good bc they make .run work so be careful)
    */

    pros::lcd::set_text(1, "Skills Start");

    std::vector<double> start_pos = {0.9, 3.3}; // wrong for now
    const double kSTARTING_ANGLE = 180.0;
    const double kSTART_TIME = pros::millis();
    const double kRECIEVE_FIRST_TIME = 20000; // how long should get triballs at first pos for in millis
    const double kSKILLS_TIME = 25000;

    ast_odom.initTracker(start_pos[0], start_pos[1], kSTARTING_ANGLE);
    pros::delay(50);
    pros::Task odom_task{[=] {
        while (1) {
            ast_odom.updatePosition();
            pros::delay(50);
        }
    }};

    pros::lcd::set_text(1, "Skills Drive to Goal");
    std::vector<std::vector<double>> to_goal_path = {start_pos, {2.7, 3.3}, {2.5, 2.1}};
    catcher_follow_path(to_goal_path, kSTARTING_ANGLE);
    pros::lcd::set_text(1, "At Goal");
    catcher_wings.on(); // turn wings on

    // pros::Task drive_to_goal_task {[=] {
    //     pros::lcd::set_text(1, "Skills Drive to Goal");
    //     std::vector<std::vector<double>> to_goal_path = {start_pos, {2.7, 3.3}, {2.5, 2.1}};
    //     catcher_follow_path(to_goal_path, 90);
    //     pros::lcd::set_text(1, "At Goal");
    // }};

    // while (drive_to_goal_task.get_state() != pros::E_TASK_STATE_DELETED) {
    //     if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
    //         drive_to_goal_task.suspend();
    //         break;
    //     }

    //     pros::delay(50);
    // }


    // signal to move over to next spot
    pros::Task wait_for_signal {[=] {
        pros::delay(100);
        pros::lcd::set_text(1, "Skills wait for signal");
        const int kWAIT_TIME = 5000;
        // catcher_link->waitForNotify(kWAIT_TIME);
        pros::delay(kWAIT_TIME);
    }};

    double time_since_last_push = 0;
    double last_time = pros::millis();
    const double kPUSH_IN_AFTER = 5000;

    const double kSTART_REC_TIME = pros::millis();

    // while we are still waiting push in every kPUSH_IN_AFTER seconds
    while (wait_for_signal.get_state() != E_TASK_STATE_DELETED) {
        pros::lcd::set_text(1, "Skills push in");

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


    pros::lcd::set_text(1, "Skills move to second pos");
    std::vector<std::vector<double>> to_second_score = {{ast_odom.getX(), ast_odom.getY()}, {2.5, 1.5}};
    catcher_follow_path(to_second_score, 90);
    pros::lcd::set_text(1, "Skills second push in cycling");


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

    pros::lcd::set_text(1, "Skills DONE");

    odom_task.suspend();

    // all done!
}


void test_path_funct() {
    // ast_odom.initTracker(0, 0, 0);
    // pros::delay(50);
    // pros::Task odom_task{[=] {
    //     while (1) {
    //         ast_odom.updatePosition();
    //         pros::delay(50);
    //     }
    // }};

    // std::vector<std::vector<double>> to_goal_path = {{0.0, 0.0}, {0.0, 1.2}};
    // catcher_follow_path(to_goal_path, 0);

    std::vector<double> start_pos = {0.9, 3.3}; // wrong for now
    const double kSTARTING_ANGLE = 90.0;
    const double kSTART_TIME = pros::millis();
    const double kRECIEVE_FIRST_TIME = 20000; // how long should get triballs at first pos for in millis
    const double kSKILLS_TIME = 45000;

    ast_odom.initTracker(start_pos[0], start_pos[1], kSTARTING_ANGLE);
    pros::delay(50);
    pros::Task odom_task{[=] {
        while (1) {
            ast_odom.updatePosition();
            pros::delay(50);
        }
    }};

    pros::lcd::set_text(1, "Skills Drive to Goal");
    std::vector<std::vector<double>> to_goal_path = {start_pos, {2.7, 3.3}, {2.5, 2.1}};
    catcher_follow_path(to_goal_path, 90);
    pros::lcd::set_text(1, "At Goal");

    odom_task.suspend();
}