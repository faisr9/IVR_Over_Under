#pragma once
#include "main.h"
using namespace std;
using namespace pros;

// normalize joystick values to -1 to 1
static double normalize_joystick(double input)
{
    return input / 127.0;
};
// apply smooth sin scaling
static double sin_scale(double input, double sin_scale_factor)
{
    return copysign(pow(sin((M_PI / 2) * fabs(input)), sin_scale_factor), input);
};
// apply square scaling
static double square_scale(double input)
{
    return copysign(pow(input, 2), input);
};

class drive
{
    public:
        // pure virtual functions
        virtual void op_drive() = 0;
        virtual void vector_drive() = 0;


};

class traditional_drive: public drive
{
    private:

        double left=12000, right=12000; // voltage to send to motors
        Controller *master;

    public:

    traditional_drive(Controller *mstr)
    {
        master=mstr;
        toggle_drive_mode();
    };

    traditional_drive(Controller *mstr,int mode) 
    {
        master = mstr;
        toggle_drive_mode(mode);
    };

    void toggle_drive_mode(int mode=0)
    {
        switch(mode)
        {
            case 0:
                arcade_drive();
                break;
            case 1:
                tank_drive();
                break;
            case 2:
                hybrid_drive();
                break;
            default:
                arcade_drive();
                break;
        }
    };

    void op_drive()
    {

    };

    void arcade_drive()
    {
        double fwd = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y))),
               turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)));
        left *= fwd + turn;
        right *= fwd - turn;
    };

    void tank_drive()
    {
        left *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));
        right *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)));
    };

    void hybrid_drive()
    {
        double fwd = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y))),
                turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)));
        left*=fwd-turn;
        right*=fwd+turn;
    };
    
};
