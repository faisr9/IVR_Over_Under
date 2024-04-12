#include "comp_15/pneumatics.h"

//This subsystem have three objects of the Piston class for wings and climber.
//This class is necessary because it implements toggle() method and the getStatus() method.
//get_value() from pros::ADIDigitalOutput doesn't work because VS Code says it's "inaccessible".

Pneumatics* Pneumatics::instance_ = nullptr;

bool currentPiston = false;

Pneumatics* Pneumatics::createInstance(char wings_piston, char sidehang_piston, char tophang_piston, char intake_piston) {
    if (!instance_) {
        instance_ = new Pneumatics(wings_piston, sidehang_piston, tophang_piston, intake_piston);
    }

    return instance_;
}

Pneumatics* Pneumatics::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

Pneumatics::Pneumatics(char piston_w, char piston_t, char piston_s, char piston_i) : SubsystemParent("Pneumatics"), wings(piston_w), sidehang(piston_s), tophang(piston_t), intake(piston_i){
    wings_status = false;
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
 *     wings()->off();
 * 
 * or
 * 
 *     wings()->toggle();
 * 
 * 
*/
Piston* Pneumatics::getWings(){
    return &wings;
}
/**
 * usage examples: 
 * 
 *     intake()->off();
 * 
 * or
 * 
 *     intake()->toggle();
 * 
 * 
*/
Piston* Pneumatics::getIntake(){
    return &intake;
}

/**
 * usage examples: 
 * 
 *     sidehang()->off();
 * 
 * or
 * 
 *     sidehang()->toggle();
 * 
 * 
*/
Piston* Pneumatics::getSideHang(){
    return &sidehang;
}

/**
 * usage examples: 
 * 
 *     tophang()->off();
 * 
 * or
 * 
 *     tophang()->toggle();
 * 
 * 
*/
Piston* Pneumatics::getTopHang(){
    return &tophang;
}

void Pneumatics::stop() {
    wings.off();
    tophang.off();
    sidehang.off();
    intake.off();
}
