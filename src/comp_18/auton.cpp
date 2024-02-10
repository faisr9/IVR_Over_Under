#include "comp_18/auton.h"
#include "comp_18/devices.h"
#include "common_code/movement_tank.h"
using namespace pros;
using namespace std;

void auton18() {
    //1 tile is .61 meters (2 ft)

    // 1. drop intake (activate climbing piston)
    Pneumatics::getInstance()->getClimber()->on();

    // 2. retract climbing piston
    delay(350);
    //Pneumatics::getInstance()->getClimber()->off();

    vector<double> start = vectOff(0, 0); //Start position
    Intake::getInstance()->set_power(-127 / 1.5);

    delay(75);
    vector<vector<double>> curvePath = {start, {2.90, 0.4}, {2, 1.6}};//, {2.75,1.5}};
    move(curvePath, 88, false, true);
    vector<vector<double>> curvePath2 = {curvePath.back(), {2.6, 1.6}};
    vector<vector<double>> curvePath3 = {curvePath2.back(), {2.43, 1.6}};
    vector<vector<double>> curvePath4 = {curvePath2.back(), {2.5, 1.6}};
    Intake::getInstance()->set_power(127 / 1.5);
    delay(250);
    Pneumatics::getInstance()->setLeft(1);
    followPath(curvePath2, tank_drive_18, 88, false, true, false, 0.5, 3.0, 200.0 / 3.0, 450.0 / 3.0, 30, false, 1.12);
    //Pneumatics::getInstance()->setLeft(0);
    move(curvePath3, 88, 1, true);
    
    //int counter=0;

    while(1)
    {
        //Intake::getInstance()->set_power(0);
        if (triBall())
        {
            Intake::getInstance()->set_power(127 / 1.5);
            delay(250);
            moveMotors(tank_drive_18,60, 60);
            delay(1200);
            left_drive_motors.move_velocity(0);
            right_drive_motors.move_velocity(0);
            double x=tank_drive_18.getOdom().getX(),
                y=tank_drive_18.getOdom().getY();
            vector<vector<double>> oscillate = {{x,y}, {x-.275,y}};
            //followPath(curvePath2, tank_drive_18, 88, false, true, false, 0.5, 3.0, 200.0 / 3.0, 450.0 / 3.0, 30, false, 1.12);
            move(oscillate, 88, 1, true);
        }
        delay(50);
    }

    delay(1000);
    Intake::getInstance()->set_power(0);
    return;



	vector<vector<double>> move1 = {start, vect(3.8, 0.5)}; //Movement 1 (Move to intake depot)
	vector<vector<double>> move2 = {move1.back(), vect(5, .8)}; // Movement 2 (Turn to flick triball)
    vector<vector<double>> move3 = {move2.back(), vect(4, 2.5)}; // Movement 3 (Drive forward to score)
    vector<vector<double>> move4 = {move3.back(), vect(4.5, 2.5)}; // Movement 4 (Drive back to go to pick up ball)

    // 1. drop intake (activate climbing piston)
    Pneumatics::getInstance()->getClimber()->on();

    // 2. retract climbing piston
    delay(350);
    Pneumatics::getInstance()->getClimber()->off();

    // 3. turn on intake

    Intake::getInstance()->set_power(-127/1.5);
    
    delay(100);

    lcd::set_text(1, "move 1 start");
    move(move1, 90, false, false);
    lcd::set_text(1, "move 1 end");

    delay(100);

    lcd::set_text(1, "move 2 start");
    move(move2, 5, false, true);
    lcd::set_text(1, "move 2 end");

    delay(100);
    
    Intake::getInstance()->set_power(127);

    delay(100);

    lcd::set_text(1, "move 3 start");
    turnToAngle(tank_drive_18, 30, 3.0,false, 1.12);
    move(move3,0, false, false);
    lcd::set_text(1, "move 3 end");

    //delay(100);

    turnToAngle(tank_drive_18, 90, 3.0, false, 1.12);

    lcd::set_text(1, "move 4 start");
    move(move4, 0, false, false);
    lcd::set_text(1, "move 4 end");

    delay(1000);

    Intake::getInstance()->set_power(0);
    

    // lcd::set_text(1, "move 3 start");
    // move(move3, 0, false, false);
    // lcd::set_text(1, "move 3 end");

    // lcd::set_text(1, "move 4 start");
    // move(move4, 0, true, false);
    // lcd::set_text(1, "move 4 end");

    // delay(100);

    // lcd::set_text(1, "move 4 start");
    // followPath(move4, tank_drive_18, 180, true);
}

vector<double> vect(double x, double y){
    return {(x*.61), (y*.61)};
}

vector<double> vectOff(double x, double y){
    return {(.61*2.2)+(x*.61), (.61*.5)+(y*.61)};
}

void move(vector<vector<double>> moveVec, int angle, bool isReversed, bool isSpinAtEnd)
{
    double speedfactor=3.0;
    followPath(moveVec, tank_drive_18, angle, isReversed, isSpinAtEnd, false, 0.5, 3.0, 200.0 / speedfactor, 450.0 / speedfactor, 40.0 / speedfactor, false, 1.12);
}

