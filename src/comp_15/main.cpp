// #include "comp_15/controls.h"
// #include "main.h"
// #include "common_code/movement_tank.h"
// #include "auton.h"
// #include "comp_15/include_list.h"
// #include "main.h" // Not needed, but present to show its included
#include "comp_15/auton.h"
#include "comp_15/controls.h"

extern LinkHelper* comp15link;

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	// comp15link->init();
	// imu.reset(); // Very important!!!
	// horizontal_track_adi.reset();
	// vertical_track_adi.reset();
	// gui::gui_init();
	pros::delay(1000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() 
{
	if(CompetitionCatapult::getInstance()->get_switch_state())
		CompetitionCatapult::getInstance()->set_cata_mode("R");
}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
	if(gui::selected_auton == gui::AUTON_COMP) {
		auton_15(45000, false);
		// ctrl_master.rumble("---");
	}
	else if(gui::selected_auton == gui::AUTON_SKILLS) {
		skills_15(false);
		// ctrl_master.rumble("-");
	}
	else {
		auton_15(45000, false);
		// ctrl_master.rumble("---");
	}
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	// if(gui::selected_auton == gui::AUTON_COMP) {
	// 	controls();
	// 	// ctrl_master.rumble("---");
	// }
	// else if(gui::selected_auton == gui::AUTON_SKILLS) {
	// 	skills_15(true);
	// 	controls();
	// 	// ctrl_master.rumble("-");
	// }
	// else {
	// 	controls();
	// }
	pros::Motor left_front(1, GEARSET_18);
	pros::Motor left_back(2);
	pros::Motor right_front(10, GEARSET_18);
	pros::Motor right_back(4);

	// drive_builder calssss(drive_builder::HOLONOMIC, &ctrl_master)
	// 	.with_motors(&left_front, &left_back, &right_front, &right_back)
	// 	.add_ctrl_deadzone(10, 10)
	// 	.set_default_drive_mode(drive_builder::HOLONOMIC_SR)
	// 	.init();

	// pros::Task lcd_task([=] {
	// 	while(true) {
	// 		pros::lcd::print(0, "r_x: %d", ctrl_master.get_analog(RIGHT_X_AXIS));
	// 		pros::lcd::print(1, "r_y: %d", ctrl_master.get_analog(RIGHT_Y_AXIS));
	// 		pros::lcd::print(2, "l_x: %d", ctrl_master.get_analog(LEFT_X_AXIS));
	// 		pros::lcd::print(3, "l_y: %d", ctrl_master.get_analog(LEFT_Y_AXIS));
	// 		pros::delay(50);
	// 	}
	// });

	// driveClass::driveBuilder build = driveClass::buildDrive(driveClass::HOLONOMIC, &ctrl_master);
	driveClass *mainDrive = driveClass::buildDrive(driveClass::TANK_c, &ctrl_master)
		.with_motors(&left_front, &left_back, &right_front, &right_back)
		// .add_ctrl_deadzone(10, 10)
		.use_square_scaling()
		.set_default_drive_mode(driveClass::TANK_m)
		// .add_straight_drive_scale(1.0, 1.0)
		.init();

	// cout << "Drive class created" << endl;
	mainDrive->start_drive();
	
}