#pragma once
#include "dookie/devices.h"

class Pneumatics : public SubsystemParent {
    public:
        static Pneumatics* createInstance(char wings_piston, char sidehang_piston, char tophang_piston, char intake_piston);

        static Pneumatics* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~Pneumatics();
        Pneumatics(const Pneumatics& other) = delete;

        void stop();

        Piston* getWings();
        Piston* getIntake();
        Piston* getSideHang();
        Piston* getTopHang();

    private:
        Pneumatics(char wings_piston, char sidehang_piston, char tophang_piston, char intake_piston);

        //instance_ is set to nullptr in cpp file
        static Pneumatics* instance_;

        bool wings_status;
        Piston wings, intake, tophang, sidehang;
};