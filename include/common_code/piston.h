#pragma once
#include "include_list.h"

class Piston {
    public:

        Piston(char port_value);

        void on();

        void off();

        void toggle();

        bool getStatus();

        int getToggleCount();

    private:
        pros::ADIDigitalOut piston;
        bool status;
        int toggle_count = 1;

};