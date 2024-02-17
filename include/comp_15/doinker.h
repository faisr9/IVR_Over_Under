#pragma once
#include "common_code/include_list.h"
#include "comp_15/cata.h"

class DoinkerClass : public SubsystemParent
{
    private:
        DoinkerClass(pros::Motor& subsystem_motor, pros::ADIPotentiometer& doinker_pot);
        pros::Motor doinker_motor_;
        pros::ADIPotentiometer doinker_pot_;
        static DoinkerClass* instance_;

        const int doinker_pot_min = 0;
        const int doinker_pot_low = 500;
        const int doinker_pot_down = 1400;
        const int doinker_pot_up = 2500;
        const int doinker_pot_max = 4095;
        const int doinker_pot_tolerance = 500;

        int doinker_speed;
        int doinker_pos;
        bool doinkerState;
        bool doinkerRunning;


    public:
        typedef enum doinker_move
        {
            UP,
            DOWN
        } doinker_move_t;

        DoinkerClass(const DoinkerClass& other) = delete;
        ~DoinkerClass();
        static DoinkerClass* createInstance(pros::Motor& motor, pros::ADIPotentiometer& doinker_pot);
        static DoinkerClass* getInstance();

        /**
         * Move the doinker to the top position
         * 
         * @param move The direction to move the doinker
         */
        void move(doinker_move_t move);

        /**
         * Stop the doinker
         */
        void stop();

        /**
         * Cycle the doinker
         */
        void doink();

        /**
         * Move the doinker to a specific position
         * 
         * @param pos The position to move the doinker to
         * Range: 1300 - 4095
         */
        void move_to_pos(int pos);

        /**
         * Set the speed of the doinker
         * 
         * @param speed The speed to set the doinker to
         * Range: -100 - 100 rpm
         */
        void set_speed(int speed);

        /**
         * Get the position of the doinker
         * 
         * @return The position of the doinker
         *  Range: 0 - 4095
         */
        int get_pos();
};

void move_if_cata(DoinkerClass::doinker_move_t move);