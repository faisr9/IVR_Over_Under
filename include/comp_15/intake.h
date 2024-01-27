#pragma once
#include "main.h"
//positive power goes in and negative goes out;
class IntakeClass : public SubsystemParent
{
    public:
        IntakeClass(const IntakeClass& other) = delete;
        ~IntakeClass();
        static IntakeClass* createInstance(pros::Motor& motor);
        static IntakeClass* getInstance();
        void stop();
        void set_power(int power);

    private:
        IntakeClass(pros::Motor& subsystem_motor) : SubsystemParent("Intake"), intake_motor_(subsystem_motor){};
        pros::Motor intake_motor_ = pros::Motor(14);
        static IntakeClass* instance_;
};