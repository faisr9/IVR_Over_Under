#pragma once
#include "main.h"

class TestMotors {
    public:
        TestMotors(pros::Motor &mtr) : motor(&mtr) {}
        void move() {motor->move_velocity(10);}

    private:
        pros::Motor* motor;
};