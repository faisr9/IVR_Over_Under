#pragma once
#include "PID.h"

PID::PID(double kp, double ki, double kd, double min, double max){
    pid_consts.kP = kp;
    pid_consts.kI = ki;
    pid_consts.kD = kd;
    pid_state.out_min = min;
    pid_state.out_max = max;
}

//Set kP
void PID::set_kP(double kp){
    pid_consts.kP = kp;
}

//Set kI
void PID::set_kI(double ki){
    pid_consts.kI = ki;
}

//Set kD
void PID::set_kD(double kd){
    pid_consts.kD = kd;
}

void PID::setConstants(double kp, double ki, double kd){
    pid_consts.kP = kp;
    pid_consts.kI = ki;
    pid_consts.kD = kd;
}

PID::PID_state_s PID::getState(){
    return pid_state;
}

PID::PID_consts_s PID::getConstants(){
    return pid_consts;
}

double PID::updatePID(double target, double current, double tolerance){
    pid_state.error = target-current;
    if(std::abs(pid_state.error) > tolerance){        
        pid_state.error = target - current;
        pid_state.integral += pid_state.error;
        //Ensures integral doesn't get too large
        if((pid_state.error > 0 && pid_state.lastError < 0) || (pid_state.error < 0 && pid_state.lastError > 0))
            pid_state.integral = 0;
        //Update derivative and lastError
        pid_state.derivative = pid_state.error - pid_state.lastError;
        pid_state.lastError = pid_state.error; 
        pid_state.velocity = pid_consts.kP * pid_state.error + pid_consts.kI * pid_state.integral + pid_consts.kD * pid_state.derivative;
        //Ensures output doesn't exceed max/min (saturation)
        if(pid_state.velocity > pid_state.out_max){
            pid_state.velocity = pid_state.out_max;
            pid_state.saturated = true;
        } else if(pid_state.velocity < pid_state.out_min){
            pid_state.velocity = pid_state.out_min;
            pid_state.saturated = true;
        } else {
            pid_state.saturated = false;
        }
        return pid_state.velocity;
    }
    saturation = true;
    return 0;
}