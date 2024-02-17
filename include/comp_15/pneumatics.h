#pragma once
// #include "common_code/piston.h"
// #include "common_code/include_list.h"
#include "comp_15/devices.h"

class Pneumatics : public SubsystemParent {
    public:
        static Pneumatics* createInstance(char wings_port, char floor_brake_port);
        static Pneumatics* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~Pneumatics();
        Pneumatics(const Pneumatics& other) = delete;

        void stop();

        Piston* getWings();
        Piston* getFloorBrake();


    private:
        Pneumatics(char piston1, char piston2);

        //instance_ is set to nullptr in cpp file
        static Pneumatics* instance_;

        Piston wings;
        Piston floor_brake;
};
