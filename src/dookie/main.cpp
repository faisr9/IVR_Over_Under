#include "dookie/controls.h"
#include "dookie/auton.h"

pros::Task *controlsTask;
pros::Task *loadBarTask;

const bool matchMode = false; // Set to true when in competition

/* First method to run when program starts */
void initialize() {
	// pros::lcd::initialize(); // Temp until custom GUI
	lv_style_t bar_Style;
	lv_style_copy(&bar_Style, &lv_style_plain);
	bar_Style.body.main_color = LV_COLOR_MAKE(255, 0, 0);
	bar_Style.body.grad_color = LV_COLOR_MAKE(255, 0, 0);
	bar_Style.body.radius = 1;

	lv_obj_t *loadBar = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(loadBar, LV_BTN_STYLE_REL, &bar_Style);
	lv_btn_set_style(loadBar, LV_BTN_STYLE_PR, &bar_Style);
	lv_obj_set_size(loadBar, 5, 150);
	lv_obj_align(loadBar, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);

	// Prevents IMU calibration, if program recycled during competition
	if(!pros::competition::is_connected() || !matchMode) {
		loadBarTask = new pros::Task{[&] {
			int i = 5;
			int t = 0;
			const int init_time = 5000; /** WARNING: CHANGING THIS COULD RESULT IN DATA ABORT */
			while(t < init_time) {
				lv_obj_set_size(loadBar, i, 150);
				i += 4;
				t += 20;
				pros::delay(20);
			}
			waitUntil(!imu.is_calibrating());
		}};

		imu.reset(true); // Very important!!!
	}

    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
	Pneumatics::getInstance()->getWings()->off();
	Pneumatics::getInstance()->getIntake()->off();
	Pneumatics::getInstance()->getSideHang()->off();
	Pneumatics::getInstance()->getTopHang()->off();

	if(loadBarTask != nullptr) {
		if(loadBarTask->get_state() != pros::E_TASK_STATE_INVALID 
		|| loadBarTask->get_state() != pros::E_TASK_STATE_DELETED)
			loadBarTask->remove();
		lv_obj_del(loadBar);
	}

	// if(!pros::competition::is_connected() || !matchMode)
		gui::gui_init();
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {
	if(controlsTask != nullptr)
		if(controlsTask->get_state() != pros::E_TASK_STATE_INVALID) {controlsTask->suspend();}

	if(matchMode) {
		ctrl_master.rumble(".."); 
		imu.reset(true);
		ctrl_master.rumble("--");
	}
}

/* Autonomous method */
void autonomous() {
	if(gui::selected_auton == gui::AUTON_WP) {
		main_auton(true);
	} else if(gui::selected_auton == gui::AUTON_ELIM) {
		main_auton(false);
	}  else {
		main_auton(false); // Default to win point auton
	}

	// ctrl_master.rumble("..--..");
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(0, "Ready");

	if(controlsTask == nullptr) 
		controlsTask = new pros::Task{[=] {controls();}, "Controls Task"};

	if(controlsTask->get_state() == pros::E_TASK_STATE_SUSPENDED) 
		controlsTask->resume();
}


/** /// ROBOT MATCH CHECKLIST ////
 * ROBOTSETUP:
 * - Battery greater than 50%
 * - Both climbs pistons set in shaft collars
 * - Both air tanks are full
 * - Pookie Intake bound up
 * 
 * FIELDSETUP:
 * - Use jig to align robots
 * - Bias load bars in direction of offense zone
 * - Place alliance triballs point down in load
 * 		zone corner
 * - Double check accurate placement of triballs
 * 		on starting side
 * 
 * PROGRAMSETUP:
 * - Do NOT connect to field controller
 * - Select Program (Slot 1 - IVR_Worlds)
 * - Wait for IMU to calibrate
 * - Select Auton (Win Point or Elim)
 * - Wait for head ref to approach field
 * - Connect to field controller
 * 		- Wait for two long buzzes
 * - IF DATA ABORT - Just restart program * 
*/