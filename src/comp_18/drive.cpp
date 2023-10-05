#pragma once

#include "comp_18/drive.h"

//Need to define drive_modes and drive_mode_idx

void op_drive(){}
void toggle_drive_mode(){}
std::string get_drive_name(){
    return "Todo";
}

void tank_drive(){}
void arcade_drive(){}
void hybrid_drive(){}


// Utility functions
double normalize_joysticks(double input){
    return 0.0;
}
double sin_scale(double input){
    return 0.0;
}
double square_scale(double input){
    return 0.0;
}
