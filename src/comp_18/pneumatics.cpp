#include "comp_18/pneumatics.h"

//This subsystem have three objects of the Piston class for wings and climber.
//This class is necessary because it implements toggle() method and the getStatus() method.
//get_value() from pros::ADIDigitalOutput doesn't work because VS Code says it's "inaccessible".

Pneumatics* Pneumatics::instance_ = nullptr;

bool currentPiston = false;

Pneumatics* Pneumatics::createInstance(char left_piston, char right_piston, char piston_climber) {
    if (!instance_) {
        instance_ = new Pneumatics(left_piston, right_piston, piston_climber);
    }

    return instance_;
}

Pneumatics* Pneumatics::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

Pneumatics::Pneumatics(char piston_l, char piston_r, char piston_c) : SubsystemParent("Pneumatics"), leftWing(piston_l), rightWing(piston_r), climber(piston_c){

}

//destructor deallocates instance_
Pneumatics::~Pneumatics() {
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}


/** if OnOff = 0, wing will close, if OnOff = 1, wing will open. Closing a wing that is already closed will not close the
 *  other wing if it is open. (ie if rightWing is open, setLeft(0) will not close it but setLeft(1) would)
 * usage examples: 
 * 
 *     setLeft(0);
 * 
 * or
 * 
 *     getWings()->toggle();
 * 
 * 
*/
void Pneumatics::setLeft(bool OnOff) {
    if(OnOff) {
        rightWing.off();
        pros::delay(250);
        leftWing.on();
    }
    else leftWing.off();
}
void Pneumatics::setRight(bool OnOff) {
    if(OnOff) {
        leftWing.off();
        pros::delay(250);
        rightWing.on();
    }
    else rightWing.off();
}
// will turn the opposite one off just in case
bool Pneumatics::toggleLeft() {
    if (rightWing.getStatus()) {
        rightWing.off();
        pros::delay(250);
    }
    leftWing.toggle();
    return leftWing.getStatus();    
}
bool Pneumatics::toggleRight() {
    if (leftWing.getStatus()) {
        leftWing.off();
        pros::delay(250);
    }
    rightWing.toggle();
    return rightWing.getStatus();    
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
    leftWing.off();
    rightWing.off();
    climber.off();
}
