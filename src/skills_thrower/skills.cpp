#include "skills_thrower/skills.h"

LinkHelper* catcher_link = LinkHelper::createInstance(16, E_LINK_RX);


void cata_shoot_task() {
    SkillsCata* cata_inst = SkillsCata::getInstance();
    const int kNUM_FIRE = 2;

    for (int cycle_num = 0; cycle_num < kNUM_FIRE; cycle_num++) {
        cata_inst->cycle(false);
        cata_inst->cycle(true);
        pros::delay(3000);
    }
    cata_inst->stop();
    
}
double meters(double inch)
{
    return inch*.0254;
}

void skills() {

    // Setup

    const double kP = 2.8;
    std::vector<double>  init= {meters(20), meters(20)};
    
    std::vector<double> start_pos = {meters(26.5), 0.165};
    std::vector<double> launch_pos={.35,.35};//{meters(16.5),meters(19)};//{.45,.45};
    const double kSTARTING_ANGLE = 90.0; // 45˚ chosen for ease of setup, 60˚ is the actual angle
    const double kSHOOT_ANGLE = 60.0;

    x_drive_odom.initTracker(start_pos[0], start_pos[1], kSTARTING_ANGLE);
    pros::delay(50);
    pros::Task odom_task{[=] {
        while (1) {
            x_drive_odom.updatePosition();
            pros::delay(50);
        }
    }};

        //std::vector<std::vector<double>> path_to_load = {start_pos, init, launch_pos};
        std::vector<std::vector<double>> path1={start_pos,init};
        std::vector<std::vector<double>> path2={init,launch_pos};
        
        //followPathX(path_to_load, xdriveThrower, x_drive_odom, kSHOOT_ANGLE, true, false, 0.2);
        followPathX(path1, xdriveThrower, x_drive_odom, kSHOOT_ANGLE, true, false, 0.2,5.0,200,200,100);
        followPathX(path2, xdriveThrower, x_drive_odom, kSHOOT_ANGLE, true, false, 0.2,3,200,200,40);

    // pros::Task test_path_task {[=] {
    //     std::vector<std::vector<double>> straight_path = {start_pos, {0, 1.8}};
    //     followPathX(straight_path, xdriveThrower, x_drive_odom, 180, true, false, 0.2, 3.0, 200, 200);
    // }};


    ////////////////////////////////
    // SHOOT ROUTINE 1
    ////////////////////////////////

    pros::Task cata_shoot_task_1 {[=] {
        cata_shoot_task();
    }};

    std::vector<double> target_pos = {0.45, 0.45};
    // a task that runs while the catapult is firing to maintain the position of the robot
    pros::Task cata_adjustment_task_1 {[=] {
        const std::pair<double, double> kTARGET_POS = {target_pos[0], target_pos[1]};

        // run pid stuff in a loop (todo once other stuff works)
        while (1) {
            // do something inspiring
            pros::lcd::set_text(1, std::to_string(kTARGET_POS.second));
            pros::delay(500);
        }
    }};


    // how to pass args into task? idk man
    // std::pair<std::pair<double, double>, double> cata_task_args = {{1.0, 2.0}, 3.0};
    // // pros::task_fn_t* test1(&cata_adjustment_cont);
    // pros::Task cata_adjustment_task(&cata_adjustment_cont, (void*) &cata_task_args, "test");

    while (cata_shoot_task_1.get_state() != pros::E_TASK_STATE_DELETED) {
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
            cata_shoot_task_1.suspend();
            // SkillsCata::getInstance()->set_cata_mode(SkillsCata::CataMode::Cycle); // cycle once to get back to a known position
            break;
        }
        // wait here until cata task is done
        pros::delay(50); 
    }
    cata_adjustment_task_1.suspend();

    ////////////////////////////////
    // DRIVE TO OTHER GOAL
    ////////////////////////////////

    target_pos = {0.5, 3.1};

    pros::Task to_other_goal_task {[=] {
        // catcher_link->notify();
        std::vector<std::vector<double>> path_to_other_load = {start_pos, {0.9, 0.9}, {0.9, 2.7}, target_pos};
        const double kNEXT_SHOT_ANGLE = 120.0;
        followPathX(path_to_other_load, xdriveThrower, x_drive_odom, kNEXT_SHOT_ANGLE, true, false, 0.2);
    }};
    
    while (to_other_goal_task.get_state() != pros::E_TASK_STATE_DELETED) {
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
            to_other_goal_task.suspend();
            xdriveThrower.stop();
            break;
        }
        pros::delay(100);
    }

    ////////////////////////////////
    // SECOND SHOOTING ROUTINE
    ////////////////////////////////

    pros::Task cata_shoot_task_2 {[=] {
        cata_shoot_task();
    }};

    pros::Task cata_adjustment_task_2 {[=] {
        // don't set x and y vars like this bc the reference will update and these won't
        const std::pair<double, double> kTARGET_POS = {target_pos[0], target_pos[1]};

        // run pid stuff in a loop (todo once other stuff works)
        while (1) {
            // do something inspiring
            pros::lcd::set_text(1, std::to_string(kTARGET_POS.second));
            pros::delay(500);
        }
    }};

    while (cata_shoot_task_2.get_state() != pros::E_TASK_STATE_DELETED) {
        pros::delay(100);
        // if we canceled driving over then we aren't gonna be shooting, so use get_digital so the old press triggers this too
        // above is invalid but will be valid later. for now is a different button so I can tell if the path properly finished
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) { 
            cata_shoot_task_2.suspend();
            // SkillsCata::getInstance()->set_cata_mode(SkillsCata::CataMode::Cycle); // cycle once to get back to a known position
            break;
        }
    }
    cata_adjustment_task_2.suspend();

    pros::lcd::set_text(1, "All done!");

    // Cleanup

    odom_task.suspend();

    // what could possibly go wrong
}