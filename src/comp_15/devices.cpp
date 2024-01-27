#include "comp_15/comp15_includeList.h"

/**
 * Future Update:
 * When the graphics interface is implemented, a devices class will be used to manage
 * all of the devices on the robot. This will allow the port changer on the robot to
 * be active incase a port is damaged.
*/

// Controllers //
pros::Controller ctrl_master (CONTROLLER_MASTER);

// Motors //
pros::Motor FrontTopRight       (11, GEARSET_06, true);
pros::Motor FrontBottomRight    (12, GEARSET_06);
pros::Motor BackRight           (13, GEARSET_06);
pros::Motor FrontTopLeft        (20, GEARSET_06);
pros::Motor FrontBottomLeft     (19, GEARSET_06, true);
pros::Motor BackLeft            (18, GEARSET_06, true);

pros::MotorGroup RightDrive ({FrontTopRight, FrontBottomRight, BackRight});
pros::MotorGroup LeftDrive  ({FrontTopLeft, FrontBottomLeft, BackLeft});

pros::Motor cata_right  (15, GEARSET_36, true);
pros::Motor cata_left   (17, GEARSET_36);

pros::MotorGroup Cata   ({cata_right, cata_left});

pros::Motor Intake  (14, GEARSET_06, true);

// V5 Sensors //
pros::Imu imu(21);

// Legacy Sensors //
pros::ADIButton     Cata_limit  ('A');
pros::ADIDigitalOut Floor_brake ('G');
pros::ADIDigitalOut Wings       ('H');

// Traditional Drive
traditional_drive drive(imu, ctrl_master, LeftDrive, RightDrive, 0);