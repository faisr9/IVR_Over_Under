#include "PID.h"
#include "main.h"

//Sensors
pros::Rotation yEnc(10);
pros::Rotation xEnc(14);
pros::IMU imu(20);

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

void PID::setConstants(double kp, double ki, double kd, double turnKp, double turnKi, double turnKd){
    KP = kp;
    KI = ki;
    KD = kd;
    turnKP = turnKp;
    turnKI = turnKi;
    turnKD = turnKd;
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

void PID::comboPID(double target, int angle){
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

