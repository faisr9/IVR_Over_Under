#pragma once
#include "main.h"

class CompetitionCatapult : public SubsystemParent {
    public:
        static CompetitionCatapult* createInstance(pros::MotorGroup& motorgroup, pros::ADIButton& killswitch, int load, int launch);
        static CompetitionCatapult* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~CompetitionCatapult();
        CompetitionCatapult(const CompetitionCatapult& other) = delete;

        void stop();
        void prime();
        void cycle();
        void release();


    private:
        // reference to passed in motor so it doesn't create a new motor object 
        CompetitionCatapult(pros::MotorGroup& motorgroup, pros::ADIButton& killswitch, int load, int launch);

        //instance_ is set to nullptr in cpp file
        static CompetitionCatapult* instance_;

        pros::MotorGroup& motors;
        pros::ADIButton& kill_switch;
        int load_voltage;
        int launch_voltage;
};
