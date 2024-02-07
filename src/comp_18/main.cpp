#include "comp_18/comp18_includeList.h"

#include "common_code/generic_rotation_vex_rot.h"
#include "common_code/odom.h"
Imu imu(21);
pros::Rotation sensor1(9);
pros::Rotation sensor2(10);
Generic_Rotation_VEX_Rot trans_test_wheel(sensor1, 1.96 * 0.0254 / 2);
Generic_Rotation_VEX_Rot rad_test_wheel(sensor2, 1.96 * 0.0254 / 2);
Odom odometer(imu, &trans_test_wheel, &rad_test_wheel);
Controller master(E_CONTROLLER_MASTER);

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset(); // Very important!!!
	pros::delay(3000);
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

	imu.reset();
    delay(5000);

    while(true) {
        
        int forward = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y); // sets forward to left analog's up/down input
        int steer = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);  // sets steer to right analog's left/right input

        left_drive_motors.move(forward+steer);
        right_drive_motors.move(forward-steer);

        odometer.updatePosition();
        pros::lcd::print(1,"X: %lf",odometer.getX());
        pros::lcd::print(2,"Y: %lf",odometer.getY());
        pros::lcd::print(3,"Heading: %lf",odometer.getHeading());
        // pros::lcd::print(6,"1 meter: %lf",odometer.toMeters(2302.94, 0.0248));
        delay(30);
    }
}
