#include "comp_18/comp18_includeList.h"


//This subsystem have three objects of the Piston class for wings and climber.
//This class is necessary because it implements toggle() method and the getStatus() method.
//get_value() from pros::ADIDigitalOutput doesn't work because VS Code says it's "inaccessible".

Pneumatics* Pneumatics::instance_ = nullptr;

bool currentPiston = false;

Pneumatics* Pneumatics::createInstance(char piston_wings, char piston_climber) {
    if (!instance_) {
        instance_ = new Pneumatics(piston_wings, piston_climber);
    }

    return instance_;
}


Pneumatics* Pneumatics::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

Pneumatics::Pneumatics(char piston_w, char piston_c) : SubsystemParent("Pneumatics"), wings(piston_w), climber(piston_c){

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
 *     getClimber()->off();
 * 
 * or
 * 
 *     getClimber()->toggle();
 * 
 * 
*/
Piston* Pneumatics::getClimber(){
    return &climber;
}

void Pneumatics::stop() {
    wings.off();
    climber.off();
}
