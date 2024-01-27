#pragma once
#include "main.h"
//positive power goes in and negative goes out;
class Intake : public SubsystemParent
{
    public:
        Intake(const Intake& other) = delete;
        ~Intake();
        static Intake* createInstance(pros::Motor& motor);
        static Intake* getInstance();
        void stop();
        void set_power(int power);

    private:
        Intake(pros::Motor& subsystem_motor) : SubsystemParent("Intake"), intake_motor_(subsystem_motor){};
        pros::Motor intake_motor_;
        static Intake* instance_;
};