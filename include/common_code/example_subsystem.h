#pragma once
#include "include_list.h"

class ExampleSubsystem : public SubsystemParent {
    public:
        static ExampleSubsystem* createInstance(pros::Motor& motor);
        static ExampleSubsystem* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~ExampleSubsystem();
        ExampleSubsystem(const ExampleSubsystem& other) = delete;

        void stop();

        void set_power(int power);
    private:
        // reference to passed in motor so it doesn't create a new motor object 
        ExampleSubsystem(pros::Motor& subsystem_motor);

        //instance_ is set to nullptr in cpp file
        static ExampleSubsystem* instance_;

        pros::Motor subsystem_motor_ = pros::Motor(14);
};
