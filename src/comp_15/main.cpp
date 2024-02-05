#include "comp_15/comp15_includeList.h"

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
    pros::lcd::print(7,"initialized");
	imu.reset();
    odometer.initTracker(0, 0, 0);
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
    
    /*Motor front_left(8);
    front_left.set_reversed(true);
    Motor front_right(9);
    Motor back_left(7);
    back_left.set_reversed(true);
    Motor back_right(10);*/   
    
    imu.reset();
    delay(5000);

    while(true) {
        
        int forward = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y); // sets forward to left analog's up/down input
        int steer = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);  // sets steer to right analog's left/right input

        LeftDrive.move(forward+steer);
        RightDrive.move(forward-steer);

        odometer.updatePosition();
        pros::lcd::print(1,"X: %lf",odometer.getX());
        pros::lcd::print(2,"Y: %lf",odometer.getY());
        pros::lcd::print(3,"Heading: %lf",odometer.getHeading());
        pros::lcd::print(6,"1 meter: %lf",odometer.toMeters(2302.94, 0.0248));
        delay(30);
    }
}
