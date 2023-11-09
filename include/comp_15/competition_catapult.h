#pragma once
#include "common_code/subsystem_parent.h"
#include "api.h"

class CompetitionCatapult : public SubsystemParent {
    public:
        static CompetitionCatapult* createInstance(pros::Motor& motor1, pros::Motor& motor2, int load, int launch);
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
        CompetitionCatapult(pros::Motor& motor1, pros::Motor& motor2, int load, int launch);

        //instance_ is set to nullptr in cpp file
        static CompetitionCatapult* instance_;

        pros::Motor& left_motor;
        pros::Motor& right_motor;
        int left_start;
        int right_start;
        int load_voltage;
        int launch_voltage;
};
