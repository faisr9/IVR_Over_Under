#include "skills_catcher/skills.h"

// LinkHelper* catcher_link = LinkHelper::createInstance(99, E_LINK_TX);
const double kTURN_P = 2.7;

void catcher_follow_path(std::vector<std::vector<double>>& path, double final_angle) {
    followPathX(path, astdriveCatcher, ast_odom, final_angle, true, false, 0.21, 3.0, 200.0, 200.0, 20.0, false, kTURN_P);
}


void bowl(int num_triballs) {
    // assume started in correct position
    // ideally do have a drive to point pid lol, wouldn't have to worry about rot for this one

    // starting position of 0.6, 3.3

    const int kSTART_ANGLE = 270;
    const int kWHACK_ANGLE = 315;

   
    int cycles = 0;
    const double kHP_WAIT_TIME = 1000;
    const double kHP_FIRST_WAIT_TIME = 1000;
    const int turn_amount = 45;
    catcher_wings.on();
    pros::delay(kHP_FIRST_WAIT_TIME);

    while (cycles < num_triballs) {
        // whack triball out of match load zone
        turnToAngleX(astdriveCatcher, ast_odom, kWHACK_ANGLE, 6.0, false, 3.0, 10);
        // turnToAngle(drive, kTURN_BACK_ANGLE - turn_amount, 6.0, false, 3.3, 10); // more p because triball is in the way and don't care as much about precision
        // turn back so the human player can place another triball
        turnToAngleX(astdriveCatcher, ast_odom, kSTART_ANGLE, 5.0, false, 2.7, 80);
        // turnToAngle(drive, kTURN_BACK_ANGLE, 5.0, false, 1.9, 80);
        pros::delay(kHP_WAIT_TIME);
        num_triballs++;
    }

}


// void push_in() {
//     catcher_wings.off();
//     turnToAngleX(astdriveCatcher, ast_odom, 90, 3.0);
//     catcher_wings.on();

//     astdriveCatcher.robot_centric_move({0.5, 0}, 0);
//     pros::delay(1000);
//     std::vector<std::vector<double>> back_path = {{ast_odom.getX(), ast_odom.getY()}, {2.5, 2.1}};
//     catcher_follow_path(back_path, 180);
//     catcher_wings.on();

//     // field centric move hopefully ({1, pi/2}, 0)
//     // delay for like 1 sec
//     // field centric move opp dir for like 2/3 sec or some tuned amount of time


//     // could use pure pursuit/pid later but only if needed
//     pros::delay(100);
// }

void push_in_v2(std::vector<double> back_point) {
    catcher_wings.on();

    // positioned at (2.2 x, 2.1 y) facing 90
    // drive diagonal to push in

    // std::vector<std::vector<double>> push_in_path {{ast_odom.getX(), ast_odom.getY()}, {}}
    // astdriveCatcher.field_centric_move({1.0, 325.0 * M_PI / 180.0}, 0); // 0 seems to be towards goal ???
    astdriveCatcher.field_centric_move({1.0, 0}, 0); // 0 seems to be towards goal ???
    pros::delay(800);

    std::vector<std::vector<double>> back2_path = {{ast_odom.getX(), ast_odom.getY()}, back_point};
    catcher_follow_path(back2_path, 90);
}


void push_in_v3(std::vector<double> back_point) {

    catcher_wings.on();

    // wait at 2.7, 2.3

    // start facing 180

    // open wings and back up into pos
    // open wings on path

    
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

    const bool should_v3 = true;

    std::vector<double> start_pos;
    double kSTARTING_ANGLE = 90.0;
    if (should_v3) {
        // start_pos = {0.6, 3.3}; // bowl
        // kSTARTING_ANGLE = 270.0;

        start_pos = {0.76, 3.3};
        kSTARTING_ANGLE = 180.0;
    } else {
        start_pos = {0.76, 3.3}; // x is with back against back part of first tile (close to wall part)
        kSTARTING_ANGLE = 90.0;
    }
    const double kSTART_TIME = pros::millis();
    const double kRECIEVE_FIRST_TIME = 20000; // how long should get triballs at first pos for in millis
    const double kSKILLS_TIME = 55000;

    ast_odom.initTracker(start_pos[0], start_pos[1], kSTARTING_ANGLE);
    pros::delay(50);
    pros::Task odom_task{[=] {
        while (1) {
            ast_odom.updatePosition();
            pros::delay(50);
        }
    }};

    pros::lcd::set_text(1, "Skills Drive to Goal");
    if (should_v3) {
        // bowl(6); // bowl
        // turnToAngleX(astdriveCatcher, ast_odom, 90.0, 4.0, false, 2.7, 100);

        std::vector<std::vector<double>> to_goal_path = {start_pos, {2.7, 3.3}, {2.7, 2.8}, {2.4, 2.1}, {2.7, 2.3}, {2.7, 2.5}};
        catcher_follow_path(to_goal_path, kSTARTING_ANGLE);
        catcher_wings.on();
    } else {
        std::vector<std::vector<double>> to_goal_path = {start_pos, {2.7, 3.3}, {2.7, 2.9}, {2.2, 2.1}};
        catcher_follow_path(to_goal_path, kSTARTING_ANGLE);
        catcher_wings.on(); // turn wings on
    }


    pros::lcd::set_text(1, "At Goal");

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
        const int kWAIT_TIME = 20000;
        // catcher_link->waitForNotify(kWAIT_TIME);
        pros::delay(kWAIT_TIME);
    }};

    double time_since_last_push = 4000; // should push in pretty fast first time
    double last_time = pros::millis();
    const double kPUSH_IN_AFTER = 6000;

    const double kSTART_REC_TIME = pros::millis();

    // while we are still waiting push in every kPUSH_IN_AFTER seconds
    while (wait_for_signal.get_state() != E_TASK_STATE_DELETED) {
        pros::lcd::set_text(1, "Skills push in");

        time_since_last_push += pros::millis() - last_time;
        last_time = pros::millis();

        if (time_since_last_push > kPUSH_IN_AFTER) {
            time_since_last_push = 0; // set 0 first so if canceled while going back don't push in again
            push_in_v2({2.2, 2.1});
            last_time = pros::millis(); // set last time last so don't account for time spent pushing
        }



        // FOR TESTING, FOR REAL SKILLS USE VEX LINK
        // if (pros::millis() > kSTART_REC_TIME + kRECIEVE_FIRST_TIME) {
        //     break;
        // }
        pros::delay(50);
    }

    pros::lcd::set_text(1, "Skills move to second pos");

    const double kLENIENT_PUSH_IN_AFTER = 2000; // something less than kPUSH_IN_AFTER to round up an extra triball
    // time should matter too much maybe bc other robot driving full field?
    if (time_since_last_push > 2000) {
        push_in_v2({2.2, 2.1});
    } else {
        // stop the drive because it was probably still running when the task was canceled
        astdriveCatcher.stop();
    }


    pros::lcd::set_text(1, "Skills move to second pos");
    std::vector<std::vector<double>> to_second_score = {{ast_odom.getX(), ast_odom.getY()}, {2.2, 1.5}};
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
            push_in_v2({2.2, 1.5});
            time_since_last_push = 0;
        }
    }

    push_in_v2({2.2, 1.5});

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

    std::vector<double> start_pos = {0.0, 0.0}; // wrong for now
    const double kSTARTING_ANGLE = 0.0;
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
    std::vector<std::vector<double>> to_goal_path = {start_pos, {0, 1.5}};
    catcher_follow_path(to_goal_path, 90);
    pros::lcd::set_text(1, "At Goal");

    odom_task.suspend();
}