#include "comp_18/comp18_includeList.h"
#include "common_code/movement_tank.h"


void auton18() {
    //1 tile is .61 meters (2 ft)
	std::vector<std::vector<double>> move1 = {vect(0, 0), vect(3, 0)}; //Movement 1 (Move to intake depot)
	std::vector<std::vector<double>> move2 = {move1.back(), vect(3.5, .5)}; // Movement 2 (Turn to flick triball)
    std::vector<std::vector<double>> move3 = {move2.back(), vect(4, 1.5)}; // Movement 3 (Drive forward to score)


    // 1. drop intake (activate climbing piston)

    // 2. retract climbing piston

    // 3. turn on intake

    pros::lcd::set_text(1, "move 1 start");
    move(move1, 90, false, false);
    pros::lcd::set_text(1, "move 1 end");

    pros::delay(100);

    pros::lcd::set_text(1, "move 2 start");
    move(move2, 300, false, false);
    pros::lcd::set_text(1, "move 2 end");

    pros::delay(100);

    pros::lcd::set_text(1, "move 3 start");
    move(move3, 0, false, false);
    pros::lcd::set_text(1, "move 3 end");

    // pros::delay(100);

    // pros::lcd::set_text(1, "move 4 start");
    // followPath(move4, tank_drive_18, 180, true);
}

std::vector<double> vect(double x, double y){
    return {(.61*2.5)+(x*.61), (.61*.5)+(y*.61)};
}

void move(std::vector<std::vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd){
    followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd,  false, 0.5, 3.0, 200.0/2, 450.0/2, 40.0/2, false);

}