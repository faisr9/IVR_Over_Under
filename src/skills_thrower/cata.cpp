//I dont remember includes
#include "cata.h"


CompetitionCatapult* CompetitionCatapult::instance_ = nullptr;

CompetitionCatapult* CompetitionCatapult::createInstance(pros::MotorGroup& motorgroup, pros::Rotation& rotation_sensor){
    if (!instance_) {
        instance_ = new CompetitionCatapult(motorgroup, rotation_sensor);
    }

    return instance_;
}
CompetitionCatapult* CompetitionCatapult::getInstance(){
    if (instance_ == nullptr) {
        throw std::runtime_error("Cata: Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}
CompetitionCatapult::~CompetitionCatapult(){
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}
void CompetitionCatapult::stop(){

}
void CompetitionCatapult::prime(){

}
void CompetitionCatapult::cycle(){

}
void CompetitionCatapult::release(){
    
}