#include "comp_18/comp18_includeList.h"
#include "common_code/movement_tank.h"


void auton18() {
    //1 tile is .61 meters (2 ft)
	std::vector<std::vector<double>> move1 = {vect(0, 0, 0), vect(3, 0, 0)}; //Movement 1 (Move to intake depot)
	std::vector<std::vector<double>> move2 = {move1.back(), vect(3.5, 0, .5)}; // Movement 2 (Turn to flick triball)
    std::vector<std::vector<double>> move3 = {move2.back(), vect(4, 0, 1.5)}; // Movement 3 (Drive forward to score)


    // 1. drop intake (activate climbing piston)

    // 2. retract climbing piston

    // 3. turn on intake

    pros::lcd::set_text(1, "move 1 start");
    followPath(move1, tank_drive_18, 90, false);
    pros::lcd::set_text(1, "move 1 end");

    pros::delay(100);

    pros::lcd::set_text(1, "move 2 start");
    followPath(move2, tank_drive_18, 300, false);
    pros::lcd::set_text(1, "move 2 end");

    pros::delay(100);

    pros::lcd::set_text(1, "move 3 start");
    followPath(move3, tank_drive_18, 0, true);
    pros::lcd::set_text(1, "move 3 end");

    // pros::delay(100);

    // pros::lcd::set_text(1, "move 4 start");
    // followPath(move4, tank_drive_18, 180, true);
}

std::vector<double> vect(double x, double y, double numTiles){
    return {(.61*2.5)+(x*.61), (.61*.5)+(y*.61)};
}