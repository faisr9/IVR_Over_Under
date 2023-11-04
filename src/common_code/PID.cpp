#include "PID.h"
#include "main.h"

//Sensors
pros::Rotation yEnc(10);
pros::Rotation xEnc(14);
pros::IMU imu(20);

PID::PID(double Kp, double Ki, double Kd){
    //FWD PID constants
    KP = Kp;
    KI = Ki;
    KD = Kd;
}

PID::PID(double Kp, double Ki, double Kd, double turnKp, double turnKi, double turnKd){
    //FWD PID constants
    KP = Kp;
    KI = Ki;
    KD = Kd;
    //Turn PID constants
    turnKP = turnKp;
    turnKI = turnKi;
    turnKD = turnKd;
}

PID::PID(double Kp, double Ki, double Kd, double turnKp, double turnKi, double turnKd, double strafeKp, double strafeKi, double strafeKd){
    //FWD PID constants
    KP = Kp;
    KI = Ki;
    KD = Kd;
    //Turn PID constants
    turnKP = turnKp;
    turnKI = turnKi;
    turnKD = turnKd;
    //Strafe PID constants
    strafeKP = strafeKp;
    strafeKI = strafeKi;
    strafeKD = strafeKd;
}

void PID::setFwdConstants(double kp, double ki, double kd){
    KP = kp;
    KI = ki;
    KD = kd;
}

void PID::setTurnConstants(double kp, double ki, double kd){
    turnKP = kp;
    turnKI = ki;
    turnKD = kd;
}

void PID::setStrafeConstants(double kp, double ki, double kd){
    strafeKP = kp;
    strafeKI = ki;
    strafeKD = kd;
}

void PID::drivePID(double target){
    double error = target;
    yEnc.reset();
    double sensorValue = 0;

    double lastError = 0;
    double derivative = 0;
    double integral = 0;

    double fwdSpd;
    while(std::abs(error) > 1){
        sensorValue = 2*M_PI*(1.96/2)*(yEnc.get_angle()/360); //2*pi*r*(degrees/360)
        
        error = target - sensorValue;

        integral += error;
        //Ensures integral doesn't get too large
        if(error=0||sensorValue>error)
            integral = 0;
        else if(error<=0)
            integral = 0;
        
        //updates derivative and lastError
        derivative = error - lastError;
        lastError = error;

        fwdSpd = KP * error + KI * integral + KD * derivative;   
        // drive(fwdSpd); //TODO: Implement drive function
    }
}

void PID::drivePID(double target, int angle){
    //FWD PID
    double error = target;
    yEnc.reset();
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
    while(std::abs(error) > 1 && std::abs(tError) > 1){

        ////////////////////////////
        //        FWD PID         //
        ////////////////////////////
        sensorValue = 2*M_PI*(1.96/2)*(yEnc.get_angle()/360); //2*pi*r*(degrees/360)
        
        error = target - sensorValue;

        integral += error;
        //Ensures integral doesn't get too large
        if(error=0||sensorValue>error)
            integral = 0;
        else if(error<=0)
            integral = 0;
        
        //updates derivative and lastError
        derivative = error - lastError;
        lastError = error;

        fwdSpd = KP * error + KI * integral + KD * derivative;  

        ////////////////////////////
        //        Turn PID        //
        ////////////////////////////
        heading = imu.get_heading(); //IMU Sensor returns current heading in degrees from 0-360
        
        tError = angle - heading;

        tIntegral += tError;
        if(tError=0||heading>tError)
            tIntegral = 0;
        else if(tError<=0)
            tIntegral = 0;
        
        tDerivative = tError - lastTError;
        lastTError = tError;

        turnSpd = turnKP * tError + turnKI * tIntegral + turnKD * tDerivative;   

        ///////////////////////////
        // drive(fwdSpd, turnSpd); //TODO: Implement drive+turn function
    }
}

void PID::drivePID(double target, int angle, double strafeTarget){
    //FWD PID
    double error = target;
    yEnc.reset();
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
    xEnc.reset();
    double sSensorValue = 0;

    double lastSError = 0;
    double sDerivative = 0;
    double sIntegral = 0;

    double strafeSpd;
    while(std::abs(error) > 1 && std::abs(tError) > 1 && std::abs(sError) > 1){

        ////////////////////////////
        //      Forward PID       //
        ////////////////////////////
        sensorValue = 2*M_PI*(1.96/2)*(yEnc.get_angle()/360); //2*pi*r*(degrees/360)
        
        error = target - sensorValue;

        integral += error;
        //Ensures integral doesn't get too large
        if(error=0||sensorValue>error)
            integral = 0;
        else if(error<=0)
            integral = 0;
        
        //updates derivative and lastError
        derivative = error - lastError;
        lastError = error;

        fwdSpd = KP * error + KI * integral + KD * derivative;  

        ////////////////////////////
        //        Turn PID        //
        ////////////////////////////
        heading = imu.get_heading(); //IMU Sensor returns current heading in degrees from 0-360
        
        tError = angle - heading;

        tIntegral += tError;
        if(tError=0||heading>tError)
            tIntegral = 0;
        else if(tError<=0)
            tIntegral = 0;
        
        tDerivative = tError - lastTError;
        lastTError = tError;

        turnSpd = turnKP * tError + turnKI * tIntegral + turnKD * tDerivative;   

        ////////////////////////////
        //        Strafe PID      //
        ////////////////////////////
        sSensorValue = 2*M_PI*(1.96/2)*(xEnc.get_angle()/360); //2*pi*r*(degrees/360)
        
        sError = strafeTarget - sSensorValue;

        sIntegral += sError;
        //Ensures integral doesn't get too large
        if(sError=0||sSensorValue>error)
            sIntegral = 0;
        else if(error<=0)
            sIntegral = 0;
        
        //updates derivative and lastError
        sDerivative = sError - lastSError;
        lastSError = sError;

        strafeSpd = strafeKP * sError + strafeKI * sIntegral + strafeKD * sDerivative;   
        
        ////////////////////////////
        // drive(fwdSpd, turnSpd, strafeSpd); //TODO: Implement drive+turn+strafe function
    }
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
        if(tError=0||heading>tError)
            tIntegral = 0;
        else if(tError<=0)
            tIntegral = 0;
        
        tDerivative = tError - lastTError;
        lastTError = tError;

        turnSpd = turnKP * tError + turnKI * tIntegral + turnKD * tDerivative;   
        // turn(turnSpd); //TODO: Implement turn function
    }
}

void PID::strafePID(double strafeTarget){
    double error = strafeTarget;
    xEnc.reset();
    double sensorValue = 0;

    double lastError = 0;
    double derivative = 0;
    double integral = 0;

    double strafeSpd;
    while(std::abs(error) > 1){
        sensorValue = 2*M_PI*(1.96/2)*(xEnc.get_angle()/360); //2*pi*r*(degrees/360)
        
        error = strafeTarget - sensorValue;

        integral += error;
        //Ensures integral doesn't get too large
        if(error=0||sensorValue>error)
            integral = 0;
        else if(error<=0)
            integral = 0;
        
        //updates derivative and lastError
        derivative = error - lastError;
        lastError = error;

        strafeSpd = strafeKP * error + strafeKI * integral + strafeKD * derivative;   
        // strafe(strafeSpd); //TODO: Implement strafe function
    }
}