#include "skills_catcher/skills.h"

void drivePID(double inches) {

}

void skills() {

    

    const double kP = 2.8;

    std::vector<double> start_pos = {0.45, 0.45};
    const double kSTARTING_ANGLE = 60.0; // tenative, not used for now
    const double kSTART_TIME = pros::millis();
    const double kRECIEVE_FIRST_TIME = 20000; // how long should get triballs at first pos for in millis

    // x_drive_odom.initTracker(start_pos[0], start_pos[1], 0);
    pros::delay(50);
    pros::Task odom_task{[=] {
        while (1) {
            // x_drive_odom.updatePosition();
            pros::delay(50);
        }
    }};


    pros::Task drive_to_goal_task {[=] {
        std::vector<std::vector<double>> to_goal_path = {start_pos, {2.7, 3.3}, {2.7, 2.2}};
        // followPathX(to_goal_path, x_drive_odom, odom, 90);
    }};

    while (drive_to_goal_task.get_state() != pros::E_TASK_STATE_DELETED) {
        if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
            drive_to_goal_task.suspend();
            // SkillsCata::getInstance()->set_cata_mode(SkillsCata::CataMode::Cycle); // cycle once to get back to a known position
            break;
        }

        pros::delay(50);
    }


    // while (pros::millis())


}