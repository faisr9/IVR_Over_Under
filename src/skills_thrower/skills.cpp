#include "skills_thrower/skills.h"

void skills() {

    const double kP = 2.8;
    std::vector<double> start_pos = {0.45, 0.45};

    x_drive_odom.initTracker(start_pos[0], start_pos[1], 0);
    pros::delay(50);
    pros::Task odom_task{[=] {
        while (1) {
            x_drive_odom.updatePosition();
            pros::delay(50);
        }
    }};

    // pros::Task test_path_task {[=] {
    //     std::vector<std::vector<double>> straight_path = {start_pos, {0, 1.8}};
    //     followPathX(straight_path, xdriveThrower, x_drive_odom, 180, true, false, 0.2, 3.0, 200, 200);
    // }};

    pros::Task to_other_goal_task {[=] {
        std::vector<std::vector<double>> path_to_other_load = {start_pos, {0.9, 0.9}, {0.9, 2.7}, {0.5, 3.1}};
        const double kNEXT_SHOT_ANGLE = 120.0;
        // turnToAngleX(xdriveThrower, x_drive_odom, kNEXT_SHOT_ANGLE, 3);
        followPathX(path_to_other_load, xdriveThrower, x_drive_odom, kNEXT_SHOT_ANGLE, true, false, 0.2);
    }};
    
    while (to_other_goal_task.get_state() != pros::E_TASK_STATE_DELETED) {
        if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)) {
            to_other_goal_task.suspend();
            xdriveThrower.stop();
            break;
        }
        pros::delay(100);
    }

    

    // odom_task.suspend();

    // what could possibly go wrong
}