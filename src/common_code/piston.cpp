#include "common_code/piston.h"


Piston::Piston(char port_value) : piston(port_value){
    status = false;
}

void Piston::on() {
    status = true;
    piston.set_value(true);
}

void Piston::off() {
    status = false;
    piston.set_value(false);
}

void Piston::toggle(){
    status = !status;
    if(status) {
        piston.set_value(1);  
    } else {
        piston.set_value(0);  
    }
}

bool Piston::getStatus(){
    return status;
}