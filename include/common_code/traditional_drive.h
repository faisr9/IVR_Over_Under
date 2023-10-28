#pragma once
#include "main.h"
#include "subsystem_parent.h"
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

class traditional_drive : public SubsystemParent
{
    private:

        double left=12000, right=12000; // voltage to send to motors
        double fwd, turn; //helper variables
        int std_delay = 20; // delay between loops (ms)
        Controller *master; // controller to get input from
        string drive_mode[3] = {"arcade", "tank", "hybrid"}; // drive mode names
        Motor_Group *left_side, *right_side; // motor groups to send voltage to

    public:
        traditional_drive(Controller *mstr, Motor_Group *l, Motor_Group *r) : traditional_drive(mstr, l, r, 0){};

        traditional_drive(Controller *mstr, Motor_Group *l, Motor_Group *r, int mode) : SubsystemParent(drive_mode[mode])
        {
            master = mstr;
            left_side = l;
            right_side = r;
            toggle_drive_mode(mode);
        };

        // toggle drive mode (arcade, tank, hybrid)
        void toggle_drive_mode(int mode=0)
        {
            // 0 = arcade, 1 = tank, 2 = hybrid
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
                    stop();
                    break;
            }
        };

        // void op_drive()
        // {

        // };

        void arcade_drive()
        {
            do{
            fwd = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));
            turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)));
            left *= fwd + turn;
            right *= fwd - turn;

            setV();

            delay(std_delay);

            }while(master->get_digital(E_CONTROLLER_DIGITAL_A)==0); // while a is not pressed (change to button of choice)
            stop();
        };

        void tank_drive()
        {
            do{
            left *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));
            right *= square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)));

            setV();

            delay(std_delay);

            }while(master->get_digital(E_CONTROLLER_DIGITAL_A)==0); // while a is not pressed (change to button of choice)
            stop();
        };

        void hybrid_drive()
        {
            do{
            fwd = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));
            turn = square_scale(normalize_joystick(master->get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)));
            left*=fwd-turn;
            right*=fwd+turn;

            delay(std_delay);
            }while(master->get_digital(E_CONTROLLER_DIGITAL_A)==0); // while a is not pressed (change to button of choice)
            stop();
        };
        
        // turn off motors
        void stop()
        {
            left=0;
            right=0;
            setV();
        };

        // set voltage to motors
        void setV() 
        {
            left_side->move_voltage(left);
            right_side->move_voltage(right);
            left=right=12000;
        };
    
};
