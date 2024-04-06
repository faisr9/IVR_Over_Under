#pragma once
#include "comp_18/devices.h"

class Pneumatics : public SubsystemParent {
    public:
        static Pneumatics* createInstance(char left_piston, char right_piston, char piston_pto, char piston_intake);

        static Pneumatics* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~Pneumatics();
        Pneumatics(const Pneumatics& other) = delete;

        void stop();

        void setLeft(bool OnOff);
        void setRight(bool OnOff);
        bool toggleLeft();
        bool toggleRight();
        bool toggleWings();

        Piston* getIntake();
        Piston* getPTO();


    private:
        Pneumatics(char left_piston, char right_piston, char piston_climber, char piston_intake);

        //instance_ is set to nullptr in cpp file
        static Pneumatics* instance_;

        Piston leftWing, rightWing;
        bool wings_status;
        Piston intake, pto;
};