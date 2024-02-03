#include "comp_15/pneumatics.h"


//This subsystem have three objects of the Piston class for wings, kickstand, and climber.
//This class is necessary because it implements toggle() method and the getStatus() method.
//get_value() from pros::ADIDigitalOutput doesn't work because VS Code says it's "inaccessible".

extern Pneumatics* Pneumatics::instance_ = nullptr;

bool currentPiston = false;

Pneumatics* Pneumatics::createInstance(char piston_wings, char piston_kickstand, char piston_climber) {
    if (!instance_) {
        instance_ = new Pneumatics(piston_wings, piston_kickstand, piston_climber);
    }

    return instance_;
}


Pneumatics* Pneumatics::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

Pneumatics::Pneumatics(char piston_w, char piston_k, char piston_c) : SubsystemParent("Pneumatics"), wings(piston_w), kickstand(piston_k), climber(piston_c){

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
Piston Pneumatics::getWings(){
    return wings;
}

/**
 * usage examples: 
 * 
 *     getClimb()->off();
 * 
 * or
 * 
 *     getClimb()->toggle();
 * 
 * 
*/
Piston Pneumatics::getClimber(){
    return climber;
}


/**
 * usage examples: 
 * 
 *     getKickstand()->off();
 * 
 * or
 * 
 *     getKickstand()->toggle();
 * 
 * 
*/
Piston Pneumatics::getKickstand(){
    return kickstand;
}

void Pneumatics::stop() {
    wings.off();
    kickstand.off();
    climber.off();
}
