#include "comp_15/devices.h"

/**
 * Future Update:
 * When the graphics interface is implemented, a devices class will be used to manage
 * all of the devices on the robot. This will allow the port changer on the robot to
 * be active incase a port is damaged.
*/

// Controllers //
pros::Controller ctrl_master (CONTROLLER_MASTER);

// Motors //
pros::Motor front_top_right       (11, GEARSET_06, true);
pros::Motor front_bottom_right    (12, GEARSET_06);
pros::Motor back_right           (13, GEARSET_06);
pros::Motor front_top_left        (20, GEARSET_06);
pros::Motor front_bottom_left     (19, GEARSET_06, true);
pros::Motor back_left            (18, GEARSET_06, true);

pros::MotorGroup right_drive ({front_top_right, front_bottom_right, back_right});
pros::MotorGroup left_drive  ({front_top_left, front_bottom_left, back_left});

pros::Motor cata_right  (15, GEARSET_36, true);
pros::Motor cata_left   (17, GEARSET_36);

pros::MotorGroup cata   ({cata_right, cata_left});

pros::Motor intake  (14, GEARSET_06, true);

// V5 Sensors //
pros::Imu imu(21);

// Traditional Drive
traditional_drive drive(imu, ctrl_master, left_drive, right_drive, 0);

// Instances
Intake* intake_instance = Intake::createInstance(intake);
//Catapult* catapult_instance = Catapult::createInstance(cata, cata_limit);
pros::ADIButton     Cata_limit  ('A');
const char FLOOR_BRAKE = 'G';
const char WINGS = 'H';
Pneumatics* pneumatics_instance = Pneumatics::createInstance(WINGS, FLOOR_BRAKE);

