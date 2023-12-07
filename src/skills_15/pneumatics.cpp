#include "skills_15/pneumatics.h"
#include <iostream>
#include "api.h"

/*
- sets instance_ as nullptr for proper memory management
- 'extern' keyword is used to link instance_ back to the class variable
*/
extern Pneumatics* Pneumatics::instance_ = nullptr;

bool currentPiston = false;

/*
- creates an object of the class in the heap
- sets instance_ to the address of the object created
- only activates if instance_ hasn't been initialized
*/
Pneumatics* Pneumatics::createInstance(char piston_wings, char piston_kickstand, char piston_climber) {
    if (!instance_) {
        instance_ = new Pneumatics(piston_wings, piston_kickstand, piston_climber);
    }

    return instance_;
}


/* 
- returns instance_
- throws a runtime error if instance_
    - 'createInstance' hasn't been called
    - instance_ has been deallocated)
*/
Pneumatics* Pneumatics::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}

//constructor initializes subsysteem_motor_ and also calls SubsystemParent constructor with subsystem name
Pneumatics::Pneumatics(char piston_w, char piston_k, char piston_c) : SubsystemParent("Pneumatics"), wings(piston_w), kickstand(piston_k), climber(piston_c){
    
}

//destructor deallocates instance_
Pneumatics::~Pneumatics() {
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}



Piston Pneumatics::getWings(){
    return wings;
}

Piston Pneumatics::getClimber(){
    return climber;
}

Piston Pneumatics::getKickstand(){
    return kickstand;
}


void Pneumatics::stop() {
    wings.off();
    kickstand.off();
    climber.off();
}
