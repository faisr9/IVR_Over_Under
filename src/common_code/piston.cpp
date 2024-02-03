#include "common_code/piston.h"


Piston::Piston(char port_value) : piston(port_value){
    status = true;
}

void Piston::on() {
    status = true;
    piston.set_value(true);
}

void Piston::off() {
    status = false;
    piston.set_value(false);
}

void Piston::toggle() {
    toggle_count++;
    pros::lcd::set_text(3, std::to_string(toggle_count));
    pros::lcd::set_text(2, std::to_string(status));
    if(status){
        off();
    }else{
        on();
    }
}

int Piston::getToggleCount(){
    return toggle_count;
}

bool Piston::getStatus(){
    return status;
}