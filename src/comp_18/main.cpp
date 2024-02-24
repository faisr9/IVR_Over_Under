#include "comp_18/controls.h"
#include "comp_18/auton.h"

extern LinkHelper* comp18link;

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	comp18link->init();
	imu.reset(); // Very important!!!
    transverse_rot_sensor.reset();
	radial_rot_sensor.reset();
	Pneumatics::getInstance()->setRight(0);
	Pneumatics::getInstance()->setLeft(0);
    pros::delay(3000);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {

	// Comp
	// auton18(45000, false);

	// Skills
	skills18(60000);
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	pros::Task odom_task{[=] {
		while (1) {
			tank_drive_18.getOdom().updatePosition();
			pros::lcd::set_text(7, "A: " + std::to_string(tank_drive_18.getOdom().getHeading()));
			pros::delay(50);
		}
	}};

	// vector<double> start= {1.1 * 0.61, 5.4 * 0.61};; //Start position
	// tank_drive_18.getOdom().initTracker(start[0], start[1], 220);

	// while(tank_drive_18.getOdom().getX() > 1.04*.61){ //2*M_PI
	// 	moveMotors(tank_drive_18, 35, 35);
	// 	pros::delay(50);
	// }
	// stopMotors(tank_drive_18);

	// Pneumatics::getInstance()->setRight(1);

	// pros::delay(250);
	// turnToAngle(tank_drive_18, 270, 10.0, false, 1.4, 50);
	// pros::delay(250);
	
	// pros::Task bowl_task {[=] {
	// 	for(int i=0; i<21; i++){
	// 		// Pneumatics::getInstance()->setRight(0);
	// 		pros::delay(250/1.5);
	// 		turnToAngle(tank_drive_18, 240, 10.0, false, 1.4, 50);
	// 		pros::delay(500/2);
	// 		// Pneumatics::getInstance()->setRight(1);
	// 		turnToAngle(tank_drive_18, 270, 10.0, false, 1.4, 50);
	// 	}
	// }};
	// while(bowl_task.get_state()!=pros::E_TASK_STATE_DELETED){
	// 	if(ctrl_master.get_digital(BUTTON_B)){
	// 		bowl_task.remove();
	// 	}
	// 	pros::delay(50);
	// }
	// Comp
	controls();

	// Skills
	// auton18(60000, true);
}