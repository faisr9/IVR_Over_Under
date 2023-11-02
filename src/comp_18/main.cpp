#include "main.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(1,"18 Comp");
	Controller master(E_CONTROLLER_MASTER);
	Motor front_left(19);
	Motor front_right(13,true);
	Motor back_left(17);
	Motor back_right(16,true);
	Imu imu(18);
	x_drive x = x_drive(master, front_left, front_right, back_left, back_right, imu);
	while(1){
		double magni = sqrt(pow(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X),2) + pow(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y),2));
		double param = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) / magni;
		double r = acos(param);
		
		if (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y) <= 0){
			r = 2*M_PI - r;
		}

		std::pair<double, double> movement_vector(magni,r);
		x.robot_centric_move(movement_vector);
		pros::delay(50);
	}
	
		
}
