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

pros::Motor cata_right  (1, GEARSET_36, true);
pros::Motor cata_left   (17, GEARSET_36);
pros::MotorGroup cata   ({cata_right, cata_left});

pros::Motor intake_motor  (15, GEARSET_06, true);

// V5 Sensors //
pros::ADIEncoder vertical_track_adi(3, 4, true); // tracking wheel #1 cd
pros::ADIEncoder horizontal_track_adi(5, 6, true); // tracking wheel #2 ef

Generic_Rotation* vertical_track = new Generic_Rotation_Digikey(vertical_track_adi, 1.96 * 0.0254 / 2);
Generic_Rotation* horizontal_track = new Generic_Rotation_Digikey(horizontal_track_adi, 1.96 * 0.0254 / 2);

pros::Imu imu(21);

// Legacy Sensors //
pros::ADIButton cata_limit('A');

// Other classes //
Odom odometry(imu, horizontal_track, vertical_track);
traditional_drive drive(imu, ctrl_master, left_drive, right_drive, odometry);

// Instances
Intake* intake_instance = Intake::createInstance(intake_motor);
CompetitionCatapult* cata_instance = CompetitionCatapult::createInstance(cata, cata_limit);

const char FLOOR_BRAKE = 'G';
const char WINGS = 'H';
Pneumatics* pneumatics_instance = Pneumatics::createInstance(WINGS, FLOOR_BRAKE);

