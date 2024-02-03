#include "PID.h"
#include "main.h"

/*
//////////////////
/// NEXT STEPS ///
//////////////////
Drive PID and turn PID aren't that different, consider making just 1 generic PID function that takes in a target - need seperate functions if certain robot dont have certain components (ex. no IMU for turnPID, no , etc.)

Consider making PID class not have both normal PID and turn PID in constructor, instead just 1 set of PID constants and if whatever system 
is calling the PID class needs drive and turn it can just make 2 PID objects

Functions in the PID class can be more generic to take in some type of supplier for the current physical position of the motor/encoder/imu. 
Based on a bit of research I did these things should exist in C++ (they do in Java as a DoubleSupplier) and we can figure out how to implement it once we need to
*/

// pros::ADIEncoder yEnc('a','b');
// pros::IMU imu(2);
// pros::Motor_Group l({1,2});
// pros::Motor_Group r({3,4});
// traditional_drive drive(imu, l, r);

// PID cnaj(1.1, 1.1, 1.1, &yEnc, 1.1, 1.1, 1.1, &imu, drive);

PID::PID(double kP, double kI, double kD, pros::ADIEncoder* yEnc, traditional_drive& tDrive):drive(tDrive){
    //FWD PID constants
    fwdPID_consts.kP = kP;
    fwdPID_consts.kI = kI;
    fwdPID_consts.kD = kD;
    //Turn PID constants (unused)
    turnPID_consts.kP = NULL;
    turnPID_consts.kI = NULL;
    turnPID_consts.kD = NULL;
    //Sensor Initialization
    this->yEnc = yEnc;
    imu = NULL; //IMU not used
}

PID::PID(double kP, double kI, double kD, pros::ADIEncoder* yEnc, double turnkP, double turnkI, double turnkD, pros::Imu* imu, traditional_drive& tDrive):drive(tDrive){
    //FWD PID constants
    fwdPID_consts.kP = kP;
    fwdPID_consts.kI = kI;
    fwdPID_consts.kD = kD;
    //Turn PID constants
    turnPID_consts.kP = turnkP;
    turnPID_consts.kI = turnkI;
    turnPID_consts.kD = turnkD;
    //Sensor Initialization
    this->yEnc = yEnc;
    this->imu = imu;
}

void PID::setFwdConstants(double kp, double ki, double kd){
    fwdPID_consts.kP = kp;
    fwdPID_consts.kI = ki;
    fwdPID_consts.kD = kd;
}

void PID::setTurnConstants(double kp, double ki, double kd){
    turnPID_consts.kP = kp;
    turnPID_consts.kI = ki;
    turnPID_consts.kD = kd;
}

void PID::drivePID(double target, int angle){
    //FWD PID//
    fwdPID_vars.error = target;
    fwdPID_vars.startValue = 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0);
    fwdPID_vars.sensorValue=0;

    fwdPID_vars.lastError = 0;
    fwdPID_vars.derivative = 0;
    fwdPID_vars.integral = 0;

    fwdPID_vars.spd = 0;

    //Turn PID//
    double heading = imu->get_heading();
    turnPID_vars.error = angle-heading;

    turnPID_vars.lastError = 0;
    turnPID_vars.derivative = 0;
    turnPID_vars.integral = 0;

    turnPID_vars.spd = 0;

    pros::Task drivePID_lt{[&]{
        while(std::abs(fwdPID_vars.error) > .3 && std::abs(turnPID_vars.error) > 1){
            ////////////////////////////
            //        FWD PID         //
            ////////////////////////////
            fwdPID_vars.sensorValue = 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0) - fwdPID_vars.startValue; //2*pi*r*(degrees/360)
            
            fwdPID_vars.error = target - fwdPID_vars.sensorValue;

            fwdPID_vars.integral += fwdPID_vars.error;
            //Ensures integral doesn't get too large
            if(fwdPID_vars.error==0||fwdPID_vars.sensorValue>fwdPID_vars.error)
                fwdPID_vars.integral = 0;
            else if(fwdPID_vars.error<=0)
                fwdPID_vars.integral = 0;
            
            //updates derivative and lastError
            fwdPID_vars.derivative = fwdPID_vars.error - fwdPID_vars.lastError;
            fwdPID_vars.lastError = fwdPID_vars.error;

            fwdPID_vars.spd = fwdPID_consts.kP * fwdPID_vars.error + fwdPID_consts.kI * fwdPID_vars.integral + fwdPID_consts.kD * fwdPID_vars.derivative;  

            ////////////////////////////
            //        Turn PID        //
            ////////////////////////////
            heading = imu->get_heading(); //IMU Sensor returns current heading in degrees from 0-360
            
            turnPID_vars.error = angle - heading;

            turnPID_vars.integral += turnPID_vars.error;
            if(turnPID_vars.error==0||heading>turnPID_vars.error)
                turnPID_vars.integral = 0;
            else if(turnPID_vars.error<=0)
                turnPID_vars.integral = 0;
            
            turnPID_vars.derivative = turnPID_vars.error - turnPID_vars.lastError;
            turnPID_vars.lastError = turnPID_vars.error;

            turnPID_vars.spd = turnPID_consts.kP * turnPID_vars.error + turnPID_consts.kI * turnPID_vars.integral + turnPID_consts.kD * turnPID_vars.derivative;   
            ///////////////////////////
            drive.move_with_power(fwdPID_vars.spd, fwdPID_vars.spd, turnPID_vars.spd);
        }
    }};
}


void PID::drivePID(double target){
    fwdPID_vars.error = target;
    fwdPID_vars.sensorValue = 0;

    fwdPID_vars.startValue = 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0);

    fwdPID_vars.lastError = 0;
    fwdPID_vars.derivative = 0;
    fwdPID_vars.integral = 0;

    fwdPID_vars.spd = 0;

    pros::Task drivePID_lt{[&]{
        while(std::abs(fwdPID_vars.error) > .3){
            fwdPID_vars.sensorValue = 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0) - fwdPID_vars.startValue; //2*pi*r*(degrees/360)
            
            fwdPID_vars.error = target - fwdPID_vars.sensorValue;

            fwdPID_vars.integral += fwdPID_vars.error;
            //Ensures integral doesn't get too large
            if(fwdPID_vars.error==0||fwdPID_vars.sensorValue>fwdPID_vars.error)
                fwdPID_vars.integral = 0;
            else if(fwdPID_vars.error<=0)
                fwdPID_vars.integral = 0;
            
            //updates derivative and lastError
            fwdPID_vars.derivative = fwdPID_vars.error - fwdPID_vars.lastError;
            fwdPID_vars.lastError = fwdPID_vars.error;

            fwdPID_vars.spd = fwdPID_consts.kP * fwdPID_vars.error + fwdPID_consts.kI * fwdPID_vars.integral + fwdPID_consts.kD * fwdPID_vars.derivative;   
            drive.move_with_power(fwdPID_vars.spd, fwdPID_vars.spd, 0);
        }
    }};
}

void PID::turnPID(int angle){
    double heading = imu->get_heading();
    turnPID_vars.error = angle-heading;

    turnPID_vars.lastError = 0;
    turnPID_vars.derivative = 0;
    fwdPID_vars.integral = 0;

    turnPID_vars.spd = 0;

    pros::Task drivePID_lt{[&]{
        while(std::abs(turnPID_vars.error) > 1){
            heading = imu->get_heading(); //IMU Sensor returns current heading in degrees from 0-360
            
            turnPID_vars.error = angle - heading;

            turnPID_vars.integral += turnPID_vars.error;
            if(turnPID_vars.error==0||heading>turnPID_vars.error)
                turnPID_vars.integral = 0;
            else if(turnPID_vars.error<=0)
                turnPID_vars.integral = 0;
            
            turnPID_vars.derivative = turnPID_vars.error - turnPID_vars.lastError;
            turnPID_vars.lastError = turnPID_vars.error;

            turnPID_vars.spd = turnPID_consts.kP * turnPID_vars.error + turnPID_consts.kI * turnPID_vars.integral + turnPID_consts.kD * turnPID_vars.derivative;   
            drive.turn_with_power(turnPID_vars.spd);
        }
    }};
}

///////////////////////////////////
// Strafing functions not needed //
///////////////////////////////////
/*
PID::PID(double Kp, double Ki, double Kd, double turnKp, double turnKi, double turnKd, double strafeKp, double strafeKi, double strafeKd){
    //FWD PID constants
    fwdPID_consts.kP = Kp;
    fwdPID_consts.kI = Ki;
    fwdPID_consts.kD = Kd;
    //Turn PID constants
    turnPID_consts.kP = Kp;
    turnPID_consts.kI = Ki;
    turnPID_consts.kD = Kd;
    //Strafe PID constants
    strafePID_consts.kP = Kp;
    strafePID_consts.kI = Ki;
    strafePID_consts.kD = Kd;
}

void PID::setStrafeConstants(double kp, double ki, double kd){
    strafePID_consts.kP = kp;
    strafePID_consts.kI = ki;
    strafePID_consts.kD = kd;
}

//////////////////////////
//////////////////////////

void PID::drivePID(double target, pros::ADIEncoder* yEnc, int angle, double strafeTarget, pros::ADIEncoder* xEnc){
    //FWD PID
    double error = target;
    double startValueY = 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0);
    double sensorValue = 0;

    double lastError = 0;
    double derivative = 0;
    double integral = 0;

    double fwdSpd;

    //Turn PID
    double heading = imu.get_heading();
    double turnPID_vars.error = angle-heading;

    double lastTError = 0;
    double tDerivative = 0;
    double turnPID_vars.integral = 0;

    double turnSpd;

    //Strafe PID
    double sError = strafeTarget;
    double startValueX = 2*M_PI*(1.96/2)*(xEnc->get_value()/360.0);
    double sSensorValue = 0;

    double lastSError = 0;
    double sDerivative = 0;
    double sIntegral = 0;

    double strafeSpd;
    pros::Task drivePID_lt{[&]{
        if(std::abs(error) > .3 && std::abs(turnPID_vars.error) > 1 && std::abs(sError) > .3){

            ////////////////////////////
            //      Forward PID       //
            ////////////////////////////
            sensorValue = startValueY - 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0); //2*pi*r*(degrees/360)
            
            error = target - sensorValue;

            integral += error;
            //Ensures integral doesn't get too large
            if(error==0||sensorValue>error)
                integral = 0;
            else if(error<=0)
                integral = 0;
            
            //updates derivative and lastError
            derivative = error - lastError;
            lastError = error;

            fwdSpd = fwdPID_consts.kP * error + fwdPID_consts.kI * integral + fwdPID_consts.kD * derivative;  

            ////////////////////////////
            //        Turn PID        //
            ////////////////////////////
            heading = imu.get_heading(); //IMU Sensor returns current heading in degrees from 0-360
            
            turnPID_vars.error = angle - heading;

            turnPID_vars.integral += turnPID_vars.error;
            if(turnPID_vars.error==0||heading>turnPID_vars.error)
                turnPID_vars.integral = 0;
            else if(turnPID_vars.error<=0)
                turnPID_vars.integral = 0;
            
            tDerivative = turnPID_vars.error - lastTError;
            lastTError = turnPID_vars.error;

            turnSpd = turnPID_consts.kP * turnPID_vars.error + turnPID_consts.kI * turnPID_vars.integral + turnPID_consts.kD * tDerivative;   

            ////////////////////////////
            //        Strafe PID      //
            ////////////////////////////
            sSensorValue = startValueX - 2*M_PI*(1.96/2)*(xEnc->get_value()/360.0); //2*pi*r*(degrees/360)
            
            sError = strafeTarget - sSensorValue;

            sIntegral += sError;
            //Ensures integral doesn't get too large
            if(sError==0||sSensorValue>error)
                sIntegral = 0;
            else if(error<=0)
                sIntegral = 0;
            
            //updates derivative and lastError
            sDerivative = sError - lastSError;
            lastSError = sError;

            strafeSpd = strafePID_consts.kP * sError + strafePID_consts.kI * sIntegral + strafePID_consts.kD * sDerivative;   
            
            ////////////////////////////
            // drive(fwdSpd, turnSpd, strafeSpd); //TODO: Implement drive+turn+strafe function
        }
    }};

//////////////////////////
//////////////////////////

void PID::strafePID(double strafeTarget, pros::ADIEncoder* xEnc){
    double error = strafeTarget;
    double sensorValue = 0;
    double startValueX = 2*M_PI*(1.96/2)*(xEnc->get_value()/360.0);

    double lastError = 0;
    double derivative = 0;
    double integral = 0;

    double strafeSpd;
    pros::Task drivePID_lt{[&]{
        if(std::abs(error) > 1){
            sensorValue = startValueX - 2*M_PI*(1.96/2)*(xEnc->get_value()/360.0); //2*pi*r*(degrees/360)
            
            error = strafeTarget - sensorValue;

            integral += error;
            //Ensures integral doesn't get too large
            if(error==0||sensorValue>error)
                integral = 0;
            else if(error<=0)
                integral = 0;
            
            //updates derivative and lastError
            derivative = error - lastError;
            lastError = error;

            strafeSpd = strafePID_consts.kP * error + strafePID_consts.kI * integral + strafePID_consts.kD * derivative;   
            // strafe(strafeSpd); //TODO: Implement strafe function
        }
    }};
}
    */

