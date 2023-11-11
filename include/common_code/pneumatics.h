#pragma once
#include "common_code/subsystem_parent.h"
#include "api.h"

class Pneumatics : public SubsystemParent {
    public:
        static Pneumatics* createInstance(pros::ADIDigitalOut& piston);
        static Pneumatics* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~Pneumatics();
        Pneumatics(const Pneumatics& other) = delete;

        void on();

        void off();

        void stop();

    private:
        // reference to passed in motor so it doesn't create a new motor object 
        Pneumatics(pros::ADIDigitalOut& piston1);

        //instance_ is set to nullptr in cpp file
        static Pneumatics* instance_;

        pros::ADIDigitalOut piston1 = pros::ADIDigitalOut(3);
};
