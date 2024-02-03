#pragma once
#include "main.h"

class CompetitionCatapult : public SubsystemParent {
    public:
        static CompetitionCatapult* createInstance(pros::MotorGroup& motorgroup, pros::ADIButton& limit_switch);
        static CompetitionCatapult* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~CompetitionCatapult();
        CompetitionCatapult(const CompetitionCatapult& other) = delete;

        void stop();
        bool prime();
        void cycle();
        bool release();
        std::string get_cata_mode();
        
        
    private:
        // reference to passed in motor so it doesn't create a new motor object 
        CompetitionCatapult(pros::MotorGroup& motorgroup, pros::ADIButton& limit_switch);

        //instance_ is set to nullptr in cpp file
        inline static CompetitionCatapult* instance_ = nullptr;

        pros::MotorGroup& motors;
        pros::ADIButton& kill_switch;
        const int cata_voltage = 100;
        // const int launch_voltage = 50;

        pros::Task cata_task;
        std::string cata_mode = "X";
};


void cata_task_funct();