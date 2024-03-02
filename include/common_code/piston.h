#pragma once
#include "main.h"

/*
Class is used to create pistons

The status of the piston is remembered a private variable in the class because Pros doesn't allow us to get the status of the ADIDigitalOut
*/
class Piston {
    public:

        /* Class constructor takes in one arguement and creates a pros::ADIDigitalOut */
        Piston(char port_value);

        void on();
        
        void off();

        void toggle();

        bool getStatus();

        int getToggleCount();

    private:
        pros::ADIDigitalOut piston_;
        bool status_ = false;
        int toggle_count_ = 0;

};