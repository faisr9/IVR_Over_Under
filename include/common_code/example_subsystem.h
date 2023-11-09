#pragma once
#include "common_code/subsystem_parent.h"
#include "api.h"

// class ExampleSubsystem {
class ExampleSubsystem : public SubsystemParent {
    public:
        static ExampleSubsystem* createInstance(pros::Motor& subsystem_motor);
        static ExampleSubsystem* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~ExampleSubsystem();
        ExampleSubsystem(const ExampleSubsystem& other) = delete;

        void set_power(int power);

        void stop();
    private:
        // reference to passed in motor so it doesn't create a new motor object 
        // (which wouldn't be the end of the world but still)
        ExampleSubsystem(pros::Motor& subsystem_motor);

        // using std::unique_ptr instead of normal pointer because it has
        // defined behvaior of always returning nullptr if no instance
        // has been assigned!
        static ExampleSubsystem* instance_;
        // static std::unique_ptr<ExampleSubsystem> instance_;

        pros::Motor& subsystem_motor_;
        const std::string kSubsystemName = "Example Subsystem";
};
