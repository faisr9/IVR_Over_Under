#include "dookie/controls.h"
#include "dookie/auton.h"

// extern LinkHelper* comp18link;

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	// comp18link->init();
	imu.reset(true); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
	Pneumatics::getInstance()->getWings()->off();
	Pneumatics::getInstance()->getIntake()->off();
	Pneumatics::getInstance()->getSideHang()->off();
	Pneumatics::getInstance()->getTopHang()->off();
	// gui::gui_init();
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

void test_auton() {
	double test_kP = 0.5;
	while(1) {
		if (ctrl_master.get_digital_new_press(BUTTON_LEFT)) {
			test_kP -= 0.01;
			printf("kP: %f\n", test_kP);
		}
		else if (ctrl_master.get_digital_new_press(BUTTON_RIGHT)) {
			test_kP += 0.01;
			printf("kP: %f\n", test_kP);
		}
		else if (ctrl_master.get_digital_new_press(BUTTON_UP)) {
			test_kP -= 0.1;
			printf("kP: %f\n", test_kP);
		} 
		else if (ctrl_master.get_digital_new_press(BUTTON_DOWN)) {
			test_kP += 0.1;
			printf("kP: %f\n", test_kP);
		}

		if (ctrl_master.get_digital_new_press(BUTTON_B)) {
			printf("Breaking\n");
			break;
		}
		else if (ctrl_master.get_digital_new_press(BUTTON_X)) {
			printf("Current Heading: %f\n", imu.get_heading());
			double startTime = pros::millis();
			turnPID(tank_drive_18, 180, 0.2, test_kP, 0, 0, 2500);
			printf("Time taken: %f\n", pros::millis() - startTime);
			delay(750);
			printf("After heading: %f\n", imu.get_heading());
		}
		else if (ctrl_master.get_digital_new_press(BUTTON_Y)) {
			printf("Resetting IMU\n");
			imu.reset(true);
			printf("Done\n");
		}

		tank_drive_18.toggle_drive_mode();
		delay(20);
	}
}

/* Autonomous method */
void autonomous() {
	lcd::clear();
	win_point_auton();
	// test_auton();
	// if(gui::selected_auton == gui::AUTON_COMP) {
		// win_point_auton();
	// }
	// else do nothing. make sure to select the auton!
}
bool lcd_trig = false;
void lcd_callback() {
	lcd_trig = true;
}

/**
 * IMPORTANT: Make a thing so while we wait for auton to start, we press and hold a button until auton
 * is ready to start. When letting go of the button, IMU recalibrates, and rumble to let us know it's ready.
*/

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	bool control_enable = true;
	lcd::register_btn2_cb(lcd_callback);
	lcd::print(0, "Ready");

	// test_auton();

	pros::Task controlsTask {[=] {controls();}};

	while(1) {
		if(ctrl_master.get_digital_new_press(BUTTON_Y)) {
			control_enable = !control_enable;
			if (control_enable) {
				controlsTask.resume();
			} else {
				controlsTask.suspend();
			}
			ctrl_master.rumble(".");
			delay(1000);
		}

		if(ctrl_master.get_digital_new_press(BUTTON_A) || lcd_trig) {
			controlsTask.suspend();
			lcd_trig = false;
			control_enable = false;
			for (int i = 0; i < 3; i++) {
				ctrl_master.rumble(".");
				delay(1000);
			}
			autonomous();
			controlsTask.resume();
			control_enable = true;
		}

		delay(100);
	}
}
