#pragma once
#include "common_code/piston.h"

class Pneumatics : public SubsystemParent {
    public:
        static Pneumatics* createInstance(char wing_left_port, char wing_right_port, char climber_port);
        static Pneumatics* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~Pneumatics();
        Pneumatics(const Pneumatics& other) = delete;

        void stop();

        Piston* getWingLeft();
        Piston* getWingRight();
        Piston* getClimber();


    private:
        Pneumatics(char piston1, char piston2, char piston3);

        //instance_ is set to nullptr in cpp file
        static Pneumatics* instance_;

        Piston wing_left;
        Piston wing_right;
        Piston climber;
};
