#include "api.h"

class Piston {
    public:

        Piston(char port_value);

        void on();

        void off();

        void toggle();


    private:
        pros::ADIDigitalOut piston;
        bool status;

};