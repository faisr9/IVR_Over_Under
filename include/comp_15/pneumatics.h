#pragma once
#include "comp_15/devices.h"

/*
Class is used to implement the pnuematics functionality of the comp_15 robot (both wings and intake)

Class constructor takes in 3 ports and creates Piston objects using each port
- left_wing_port
- right_wing_port
- intake_port

There are getter functions for each wing and intake which will return a reference to the Piston object
- getLeftWing
- getRightWing
- getIntake
*/
class Pneumatics : public SubsystemParent {
    public:
        static Pneumatics* createInstance(char left_wing_port, char right_wing_port, char intake_port);
        static Pneumatics* getInstance();

        Piston& getLeftWing();
        Piston& getRightWing();
        Piston& getIntake();

        void stop();


    private:
        Pneumatics(char left_wing_port, char right_wing_port, char intake_port);

        static Pneumatics* instance_;

        Piston left_wing_;
        Piston right_wing_;
        Piston intake_;
};
