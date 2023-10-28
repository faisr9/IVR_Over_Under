#include "api.h"

class PID{
    public:
        PID(double kp, double ki, double kd, double turnKp, double turnKi, double turnKd);
        void drivePID(double target);
        void turnPID(int angle);
        void comboPID(double target, int angle);
        void setConstants(double kp, double ki, double kd, double turnKp, double turnKi, double turnKd);

    private:    
        //Fwd PID constants
        double KP;
        double KI;
        double KD;
        //Turn PID constants
        double turnKP;
        double turnKI;
        double turnKD;
        //Strafe PID constants
        // double strafeKP;
        // double strafeKI;
        // double strafeKD;
};