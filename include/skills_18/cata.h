#pragma once
#include "main.h"

class CompetitionCatapult : public SubsystemParent {
    public:
        static CompetitionCatapult* createInstance(pros::MotorGroup& motorgroup, pros::Rotation& rotation_sensor);
        static CompetitionCatapult* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~CompetitionCatapult();
        CompetitionCatapult(const CompetitionCatapult& other) = delete;

        void stop();
        void prime();
        void cycle();
        void release();

    private:
        // reference to passed in motor so it doesn't create a new motor object 
        CompetitionCatapult(pros::MotorGroup& motorgroup, pros::Rotation& rotation_sensor) : motors(motorgroup), rotation_sensor(rotation_sensor) {};

        //instance_ is set to nullptr in cpp file
        inline static CompetitionCatapult* instance_ = nullptr;

        pros::MotorGroup& motors;
        pros::Rotation& rotation_sensor;
        int max_position_degrees = 12000; //centidegrees, not sure if should be position or angle
        const int cata_voltage = 100;
        // const int launch_voltage = 50;
};

