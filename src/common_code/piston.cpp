#include "main.h"

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

void Piston::toggle(){
    status = !status;
    piston.set_value(status);    
}

bool Piston::getStatus(){
    return status;
}