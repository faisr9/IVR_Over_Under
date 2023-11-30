#pragma once
#include "main.h"

class CompetitionCatapult : public SubsystemParent {
    public:
        static CompetitionCatapult* createInstance(pros::MotorGroup& motorgroup, pros::ADIDigitalIn& killswitch, int load, int launch);
        static CompetitionCatapult* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~CompetitionCatapult();
        CompetitionCatapult(const CompetitionCatapult& other) = delete;

        void stop();

        void set_power(int power);

        void starting_position();
        void load();
        void launch();


    private:
        // reference to passed in motor so it doesn't create a new motor object 
        CompetitionCatapult(pros::MotorGroup& motorgroup, pros::ADIDigitalIn& killswitch, int load, int launch);

        //instance_ is set to nullptr in cpp file
        static CompetitionCatapult* instance_;

        pros::MotorGroup& motors;
        pros::ADIDigitalIn& kill_switch;
        int load_voltage;
        int launch_voltage;
};
