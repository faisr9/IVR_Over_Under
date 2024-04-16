#pragma once
#include "common_code/include_list.h" //Fix this include path later when we start to make skills code



class Intake : public SubsystemParent{
    private:
        pros::Motor *intake_motor_;
        static const int default_rpm_;
        int power_;

        //Subsystem parent specific
        static Intake* instance_;

    public:
        Intake(pros::Motor &motor);

        // Sets power in rpm which will call set_power using a conversion to mV
        void set_rpm(int rpm);

        // Sets power in mV. Only this method moves the motor
        void set_power(int power);

        // Uses the default power, the power set by set_power, or the power passed in
        void toggle_on(int power = default_rpm_);

        //Subsystem parent specific
        static Intake* createInstance(pros::Motor& motor);
        static Intake* getInstance();
        void stop();
};