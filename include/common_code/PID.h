#pragma once
#include "main.h"

class PID {
    private:
        // All private variables like PID constants, sensor and/or motor pointers
        struct PID_consts_s{
            double kP;
            double kI;
            double kD;
        } pid_consts;

        // PID state variables like error, lastError, derivative, integral, etc
        // This struct can be used to store the state of the PID loop so if 
        // edge cases like saturation, etc can be handled by the user
        struct PID_state_s{
            double error;
            double lastError;
            double derivative;
            double integral;
            double velocity;
            bool saturated = false;
        } pid_state;

        // All private methods like PID calculations, sensor readings, motor movements
    public:
        PID(double Kp, double Ki, double Kd);
        ~PID(); // Any needed deconstructors

        void set_kP(double kp);
        void set_kI(double ki);
        void set_kD(double kd);
        void setConstants(double kp, double ki, double kd);

        // Getters and Setters for PID constants
        PID_consts_s getConstants();
        PID_state_s getState();

        double updatePID(double target, double current, double tolerance);
        // double runPID(double target, pros::ADIEncoder &sensor);
        // double runPID(double target, pros::IMU &sensor);

        // Other variations of runPID and other methods
        virtual void turnTo_PID(double target, double angle);
        virtual void moveTo_PID(vector<double> target, double current);
};