#include "common_code/example_subsystem.h"
#include "comp_15/competition_catapult.h"


pros::Motor test_mtr(14);

ExampleSubsystem* example_subsystem_instance = ExampleSubsystem::createInstance(test_mtr);

pros::Motor cata_left_mtr(14);
pros::Motor cata_right_mtr(14);
pros::ADIDigitalIn cata_kill_switch (14);
int cata_voltage = 14;
int cata_launch_voltage = 14;
CompetitionCatapult* competition_catapult_instance = CompetitionCatapult::createInstace(cata_left_mtr, cata_right_mtr, cata_kill_switch, cata_load_voltage, cata_launch_voltage);