#include "common_code/piston.h"


Piston::Piston(char port_value) : piston(port_value){
    status = false;
}

void Piston::on() {
    status = true;
    piston.set_value(status);
}

void Piston::off() {
    status = false;
    piston.set_value(false);
}

void Piston::toggle() {
    toggle_count++;
    if(status){
        off();
    }else{
        on();
    }
}


//this is used for testing, but could be a function good to have in case debugging needs this
int Piston::getToggleCount(){
    return toggle_count;
}

bool Piston::getStatus(){
    return status;
}