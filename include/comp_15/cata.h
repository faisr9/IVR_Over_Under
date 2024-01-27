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
        inline static CompetitionCatapult* instance_ = nullptr;

        pros::MotorGroup& motors;
        pros::ADIButton& kill_switch;
        const int load_voltage = 50;
        const int launch_voltage = 50;
};
