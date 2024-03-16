#include "PID_drive.h"

// Sensors: Temporary
pros::Rotation yEnc(10);
pros::Rotation xEnc(14);
pros::IMU imu(20);

//Methods
//pass in parameters OR pass in sensor objects
//

/*
Ways to Update:
==================
1) Keep original methods. Have them run on sperate lambda threads (requireds mutex)
2) Make PID loops once run per call
    Require a while loop in the auton file to run program
3) Directly integrate PID class in an auton class that moves robot with one method call
    passing in the sensors, motor . basically option 2
4) Have this main PID class do option 1. In the Auton Creator Class, we inherit this class
    and overload the PID methods to run custom movement integration
Best way to do this is to create PID objects per subsystem that uses PID

Things to do;
- Restructure the class so that the each PID object has its own set of PID constants.
    - We can do one where we can store multiple constants for each PID object, like the drive class
        which can have drive turn and strafe PID constants, where the tank drive will have only
        drive and turn constants
    - Create methods that pass in the target and current inputs rather than hardcoded sensor input
    - Create methods that pass in sensors?
- All PID loops should have lambda threads
- Create the varibles for the values returned by the PID fuctions. Need to loop this into mutex
- Avoid creating varibles in methods unless temp


*/

PID::PID(double Kp, double Ki, double Kd){
    //FWD PID constants
    fwdPID_consts.kP = Kp;
    fwdPID_consts.kI = Ki;
    fwdPID_consts.kD = Kd;
}

PID::PID(double Kp, double Ki, double Kd, double turnKp, double turnKi, double turnKd){
    //FWD PID constants
    fwdPID_consts.kP = Kp;
    fwdPID_consts.kI = Ki;
    fwdPID_consts.kD = Kd;
    //Turn PID constants
    turnPID_consts.kP = Kp;
    turnPID_consts.kI = Ki;
    turnPID_consts.kD = Kd;
}

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

void PID::setStrafeConstants(double kp, double ki, double kd){
    strafePID_consts.kP = kp;
    strafePID_consts.kI = ki;
    strafePID_consts.kD = kd;
}

void PID::drivePID(double target, pros::ADIEncoder* yEnc){
    double error = target;
    double sensorValue = 0;

    double startValue = 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0);

    double lastError = 0;
    double derivative = 0;
    double integral = 0;

    double fwdSpd;

    pros::Task drivePID_lt{[&]{
        if(std::abs(error) > .3){
            sensorValue = startValue - 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0); //2*pi*r*(degrees/360)
            
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
            // drive(fwdSpd); //TODO: Implement drive function
        }
    }};
}

void PID::drivePID(double target, pros::ADIEncoder* yEnc, int angle){
    //FWD PID
    double error = target;
    double startValue = 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0);
    double sensorValue=0;

    double lastError = 0;
    double derivative = 0;
    double integral = 0;

    double fwdSpd;

    //Turn PID
    double heading = imu.get_heading();
    double tError = angle-heading;

    double lastTError = 0;
    double tDerivative = 0;
    double tIntegral = 0;

    double turnSpd;

    pros::Task drivePID_lt{[&]{
        if(std::abs(error) > .3 && std::abs(tError) > 1){

            ////////////////////////////
            //        FWD PID         //
            ////////////////////////////
            sensorValue = startValue - 2*M_PI*(1.96/2)*(yEnc->get_value()/360.0); //2*pi*r*(degrees/360)
            
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
            
            tError = angle - heading;

            tIntegral += tError;
            if(tError==0||heading>tError)
                tIntegral = 0;
            else if(tError<=0)
                tIntegral = 0;
            
            tDerivative = tError - lastTError;
            lastTError = tError;

            turnSpd = turnPID_consts.kP * tError + turnPID_consts.kI * tIntegral + turnPID_consts.kD * tDerivative;   

            ///////////////////////////
            // drive(fwdSpd, turnSpd); //TODO: Implement drive+turn function
        }
    }};
}

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
    double tError = angle-heading;

    double lastTError = 0;
    double tDerivative = 0;
    double tIntegral = 0;

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
        if(std::abs(error) > .3 && std::abs(tError) > 1 && std::abs(sError) > .3){

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
            
            tError = angle - heading;

            tIntegral += tError;
            if(tError==0||heading>tError)
                tIntegral = 0;
            else if(tError<=0)
                tIntegral = 0;
            
            tDerivative = tError - lastTError;
            lastTError = tError;

            turnSpd = turnPID_consts.kP * tError + turnPID_consts.kI * tIntegral + turnPID_consts.kD * tDerivative;   

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
}

void PID::turnPID(int angle){
    double heading = imu.get_heading();
    double tError = angle-heading;

    double lastTError = 0;
    double tDerivative = 0;
    double tIntegral = 0;

    double turnSpd;
    while(std::abs(tError) > 1){
        heading = imu.get_heading(); //IMU Sensor returns current heading in degrees from 0-360
        
        tError = angle - heading;

        tIntegral += tError;
        if(tError==0||heading>tError)
            tIntegral = 0;
        else if(tError<=0)
            tIntegral = 0;
        
        tDerivative = tError - lastTError;
        lastTError = tError;

        turnSpd = turnPID_consts.kP * tError + turnPID_consts.kI * tIntegral + turnPID_consts.kD * tDerivative;   
        // turn(turnSpd); //TODO: Implement turn function
    }
}

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