#include "comp_15/pneumatics.h"
#include <iostream>
#include "api.h"


//This subsystem have three objects of the Piston class for wings and floor brake.
//This class is necessary because it implements toggle() method and the getStatus() method.
//get_value() from pros::ADIDigitalOutput doesn't work because VS Code says it's "inaccessible".

extern Pneumatics* Pneumatics::instance_ = nullptr;

bool currentPiston = false;

Pneumatics* Pneumatics::createInstance(char piston_wings, char piston_floor_brake) {
    if (!instance_) {
        instance_ = new Pneumatics(piston_wings, piston_floor_brake);
    }

    return instance_;
}


Pneumatics* Pneumatics::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

Pneumatics::Pneumatics(char piston_w, char piston_f) : SubsystemParent("Pneumatics"), wings(piston_w), floor_brake(piston_f){

}

//destructor deallocates instance_
Pneumatics::~Pneumatics() {
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}


/**
 * usage examples: 
 * 
 *     getWings()->off();
 * 
 * or
 * 
 *     getWings()->toggle();
 * 
 * 
*/
Piston* Pneumatics::getWings(){
    return &wings;
}


/**
 * usage examples: 
 * 
 *     getFloorBrake()->off();
 * 
 * or
 * 
 *     getFloorBrake()->toggle();
 * 
 * 
*/
Piston* Pneumatics::getFloorBrake(){
    return &floor_brake;
}

void Pneumatics::stop() {
    wings.off();
    floor_brake.off();
}
