#pragma once
#include "PID.h"

class drive_PID : public PID{
    private:
        struct PIDConstants{
            double kP;
            double kI;
            double kD;
        };

        PIDConstants fwdPID_consts;
        PIDConstants turnPID_consts;
        PIDConstants strafePID_consts;
    
    public:
        drive_PID(double Kp, double Ki, double Kd);
        drive_PID(double Kp, double Ki, double Kd, double turnKp, double turnKi, double turnKd);
        drive_PID(double Kp, double Ki, double Kd, double turnKp, double turnKi, double turnKd, double strafeKp, double strafeKi, double strafeKd);
        void setFwdConstants(double kp, double ki, double kd);
        void setTurnConstants(double kp, double ki, double kd);
        void setStrafeConstants(double kp, double ki, double kd);

        void drivePID(double target, pros::ADIEncoder* yEnc);
        void drivePID(double target, pros::ADIEncoder* yEnc, int angle);
        void drivePID(double target, pros::ADIEncoder* yEnc, int angle, double strafeTarget, pros::ADIEncoder* xEnc);

        void turnPID(int angle);
        void strafePID(double strafeTarget, pros::ADIEncoder *xEnc);
    
};