#include "comp_18/comp18_includeList.h"

#include "common_code/generic_rotation_vex_rot.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {

	Generic_Rotation* generic_vex_rot = new Generic_Rotation_VEX_Rot(transverse_rot_sensor, 1.96 * 0.0254 / 2);
	generic_vex_rot->initialize_sensor();
	double current_dist = 0;

	while (1) {

		if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_B)) {
			break;
		}

		double delta_distance = generic_vex_rot->get_meters_travelled();
		if (isnan(delta_distance)) {
			pros::lcd::set_text(3, "Delta distance is nan!!!!!!!");
		} else {
			pros::lcd::set_text(3, "");
			current_dist += delta_distance;
		}

		pros::lcd::set_text(2, "Current dist is " + std::to_string(current_dist));

		pros::delay(20);
	}

	delete generic_vex_rot;
}
