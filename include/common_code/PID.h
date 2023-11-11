#include "api.h"

class PID{
    private:
        struct PIDConstants{
            double kP;
            double kI;
            double kD;
        };

        PIDConstants fwdPID_consts;
        PIDConstants turnPID_consts;
        PIDConstants strafePID_consts;

        // //Fwd PID constants
        // double KP;
        // double KI;
        // double KD;

        // //Turn PID constants
        // double turnKP;
        // double turnKI;
        // double turnKD;

        // //Strafe PID constants
        // double strafeKP;
        // double strafeKI;
        // double strafeKD;
    
    public:
        PID(double Kp, double Ki, double Kd);
        PID(double Kp, double Ki, double Kd, double turnKp, double turnKi, double turnKd);
        PID(double Kp, double Ki, double Kd, double turnKp, double turnKi, double turnKd, double strafeKp, double strafeKi, double strafeKd);
        void setFwdConstants(double kp, double ki, double kd);
        void setTurnConstants(double kp, double ki, double kd);
        void setStrafeConstants(double kp, double ki, double kd);

        void drivePID(double target);
        void drivePID(double target, int angle);
        void drivePID(double target, int angle, double strafeTarget);

        void turnPID(int angle);
        void strafePID(double strafeTarget);

    
};