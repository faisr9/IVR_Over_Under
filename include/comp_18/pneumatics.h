#pragma once
#include "common_code/piston.h"

class Pneumatics : public SubsystemParent {
    public:
        static Pneumatics* createInstance(char left_piston, char right_piston, char piston_climber);

        static Pneumatics* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~Pneumatics();
        Pneumatics(const Pneumatics& other) = delete;

        void stop();

        void setLeft(bool OnOff);
        void setRight(bool OnOff);
        bool toggleLeft();
        bool toggleRight();

        Piston* getClimber();


    private:
        Pneumatics(char left_piston, char right_piston, char piston_climber);

        //instance_ is set to nullptr in cpp file
        static Pneumatics* instance_;

        Piston leftWing, rightWing;
        Piston climber;
};
