#pragma once
#include "main.h"
#include "subsystem_parent.h"
using namespace std;
using namespace pros;

class intake : public SubsystemParent {
    public:
        intake(string subsystem_name, Motor *subsystem_motor,int numMotors): SubsystemParent(subsystem_name), kSubsystemName(subsystem_name), numMot(numMotors)
        {for (int i=0; i<numMot; i++) intake_motors_[0]=(subsystem_motor+i);}; 
        //Constructor
        ~intake(); // Deconstructor

        void stop();
        string get_subsystem_name() const {return kSubsystemName;}; // Returns subsystem name
    private:
        const int numMot; // Number of motors in the subsystem
        vector<Motor*> intake_motors_; // Vector of motors in the subsystem
        const string kSubsystemName; // Name of the subsystem
};