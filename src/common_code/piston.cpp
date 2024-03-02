#include "common_code/piston.h"

Piston::Piston(char port_value) : piston_(port_value){}

void Piston::on() {
    status_ = true;
    piston_.set_value(status_);
}

void Piston::off() {
    status_ = false;
    piston_.set_value(status_);
}

void Piston::toggle() {
    toggle_count_++;
    if(status_){
        off();
    }else{
        on();
    }
}

// NOTE: This can be logged via the logger class in the future
//this is used for testing, but could be a function good to have in case debugging needs this
int Piston::getToggleCount(){
    return toggle_count_;
}

bool Piston::getStatus(){
    return status_;
}