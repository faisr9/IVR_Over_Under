#include "comp_18/controls.h"

// Will add driver profiles later
void controls() {
    pros::lcd::set_text(1, "Running Controls");
    while(1) {
        //ACTIVATE DRIVE
        tank_drive_18.toggle_drive_mode();

        //INTAKE CONTROLS
        if (ctrl_master.get_digital(BUTTON_R2)){
            Intake::getInstance()->set_power(-12000);
        }
        else if (ctrl_master.get_digital(BUTTON_R1)){
            Intake::getInstance()->set_power(12000);
        } else {
            Intake::getInstance()->set_power(0);
        }

        //PNEUMATICS CONTROLS
        if(ctrl_master.get_digital_new_press(BUTTON_L2)) {
            Pneumatics::getInstance()->toggleWings();
        }
        // if(ctrl_master.get_digital_new_press(BUTTON_R1)) {
        //     Pneumatics::getInstance()->toggleRight();
        // }
        if(ctrl_master.get_digital_new_press(BUTTON_L1)) {
            Pneumatics::getInstance()->getClimber()->toggle();
        }

        pros::delay(15);
    }
}

// void turnPID(int angle, double kP, double kD, double tolerance){
//     double heading = tank_drive_18.getOdom().getHeading();
//     double error = angle-heading;

//     double lastError = 0;
//     double derivative = 0;

//     double velocity = 0;
//     // pros::Task drivePID_lt{[&] {

//         while(std::abs(error) > tolerance){
//             heading = tank_drive_18.getOdom().getHeading(); //IMU Sensor returns current heading in degrees from 0-360
            
//             error = angle - heading;
            
//             derivative = error - lastError;
//             lastError = error;

//             velocity = kP * error + kD * derivative;   
//             // drive.turn_with_power(turnPID_vars.velocity);
// 			tank_drive_18.turn_with_power(velocity);
// 			pros::delay(10);
//         }
//     // }};
// }

// void controls()
// {
//     tank_drive_18.getOdom().initTracker(0, 0, 90);
//     pros::delay(50);

// 	pros::Task odom_task{[=] {
// 		while (1) {
// 			tank_drive_18.getOdom().updatePosition();
// 			pros::lcd::set_text(7, "A: " + std::to_string(tank_drive_18.getOdom().getHeading()));
// 			pros::delay(50);
// 		}
// 	}};

// 	while(1)
// 	{
// 		if (ctrl_master.get_digital_new_press(BUTTON_A))
// 			turnPID(90, 0.9, 0.05, 2);
		
// 		tank_drive_18.toggle_drive_mode();
// 		delay(15);
// 	}
// }
