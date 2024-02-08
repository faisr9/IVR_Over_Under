#pragma once
#include "common_code/include_list.h"

class DoinkerClass : public SubsystemParent
{
    private:
        DoinkerClass(pros::Motor& subsystem_motor, pros::ADIPotentiometer& doinker_pot);
        pros::Motor doinker_motor_;
        pros::ADIPotentiometer doinker_pot_;
        static DoinkerClass* instance_;

        const int doinker_pot_min = 0;
        const int doinker_pot_low = 1150;
        const int doinker_pot_down = 1400;
        const int doinker_pot_up = 2950;
        const int doinker_pot_max = 4095;
        const int doinker_pot_tolerance = 50;

        int doinker_speed = 100;
        int doinker_pos = 0;

    public:
        DoinkerClass(const DoinkerClass& other) = delete;
        ~DoinkerClass();
        static DoinkerClass* createInstance(pros::Motor& motor, pros::ADIPotentiometer& doinker_pot);
        static DoinkerClass* getInstance();

        void move_up();
        void move_down();
        void stop();
        void doink();
        void move_to_pos(int pos);
        void stayUp();
        void set_speed(int speed);
        int get_pos();
};