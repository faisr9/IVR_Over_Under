#pragma once
#include "common_code/subsystem_parent.h"
#include "api.h"
#include "common_code/piston.h"

class Pneumatics : public SubsystemParent {
    public:
        static Pneumatics* createInstance(char wings_port, char kickstand_port, char climber_port);
        static Pneumatics* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~Pneumatics();
        Pneumatics(const Pneumatics& other) = delete;

        void stop();

        Piston getWings();
        Piston getKickstand();
        Piston getClimber();


    private:
        // reference to passed in motor so it doesn't create a new motor object 
        Pneumatics(char piston1, char piston2, char piston3);

        //instance_ is set to nullptr in cpp file
        static Pneumatics* instance_;

        Piston wings;
        Piston kickstand;
        Piston climber;
};
