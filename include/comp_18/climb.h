#pragma once
#include "main.h"

class Climb : public SubsystemParent {
    public:
        static Climb* createInstance(pros::Motor& m1, pros::Motor& m2, double v, double mp);
        static Climb* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~Climb();
        Climb(const ExampleSubsystem& other) = delete;
        void stop();

        bool activate();

    private:
        Climb(pros::Motor& m1, pros::Motor& m2, double v, double mp);
        static Climb* instance_;
        pros::Motor motor1;
        pros::Motor motor2;
        double velocity;
        double max_position;
};