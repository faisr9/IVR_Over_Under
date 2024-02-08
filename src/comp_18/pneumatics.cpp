#include "comp_18/pneumatics.h"


//This subsystem have three objects of the Piston class for wings and floor brake.
//This class is necessary because it implements toggle() method and the getStatus() method.
//get_value() from pros::ADIDigitalOutput doesn't work because VS Code says it's "inaccessible".

Pneumatics* Pneumatics::instance_ = nullptr;

bool currentPiston = false;

Pneumatics* Pneumatics::createInstance(char piston_wing_left, char piston_wing_right, char piston_climber) {
    if (!instance_) {
        instance_ = new Pneumatics(piston_wing_left, piston_wing_right, piston_climber);
    }

    return instance_;
}


Pneumatics* Pneumatics::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

Pneumatics::Pneumatics(char piston_wl, char piston_wr, char piston_c) : SubsystemParent("Pneumatics"), wing_left(piston_wl), wing_right(piston_wr), climber(piston_c){

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
Piston* Pneumatics::getWingLeft(){
    return &wing_left;
}

Piston* Pneumatics::getWingRight(){
    return &wing_right;
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
Piston* Pneumatics::getClimber(){
    return &climber;
}

void Pneumatics::stop() {
    wing_left.off();
    wing_right.off();
    climber.off();
}
