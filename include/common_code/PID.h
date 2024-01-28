#pragma once
#include "api.h"

class PID{
    private:
        struct PIDConstants{
            double kP;
            double kI;
            double kD;
        };

        struct PIDVariables{
            double error;
            double sensorValue;
            double startValue;

            double lastError;
            double derivative;
            double integral;
        };

        PIDConstants fwdPID_consts;
        PIDConstants turnPID_consts;
        // PIDConstants strafePID_consts;

        PIDVariables fwdPID_vars;
        PIDVariables turnPID_vars;

        pros::ADIEncoder* yEnc;
        pros::Imu* imu;
        traditional_drive& drive;

    public:
        PID::PID(double Kp, double Ki, double Kd, pros::ADIEncoder* yEnc, traditional_drive& drive);
        PID::PID(double Kp, double Ki, double Kd, pros::ADIEncoder* yEnc, double turnKp, double turnKi, double turnKd, pros::Imu* imu, traditional_drive& drive);
        // PID(double Kp, double Ki, double Kd, double turnKp, double turnKi, double turnKd, double strafeKp, double strafeKi, double strafeKd);
        void setFwdConstants(double kp, double ki, double kd);
        void setTurnConstants(double kp, double ki, double kd);
        void setStrafeConstants(double kp, double ki, double kd);

        void drivePID(double target);
        void drivePID(double target, int angle);
        // void drivePID(double target, pros::ADIEncoder* yEnc, int angle, double strafeTarget, pros::ADIEncoder* xEnc);

        void turnPID(int angle);
        // void strafePID(double strafeTarget, pros::ADIEncoder *xEnc);
};