
#pragma once
#include "skills_thrower/devices.h"

class SkillsCata : public SubsystemParent {
    public:
        static SkillsCata* createInstance(pros::MotorGroup& motorgroup, pros::Rotation& rotation_sensor);
        static SkillsCata* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~SkillsCata();
        SkillsCata(const SkillsCata& other) = delete;

        void stop();
        void cycle();

        void move_forward_manual();
        std::string get_cata_mode();   

        void set_cata_mode(std::string new_cata_mode);
        void set_cata_mode_internal(std::string new_cata_mode);

    private:
        // reference to passed in motor so it doesn't create a new motor object 
        SkillsCata(pros::MotorGroup& motorgroup, pros::Rotation& rotation_sensor);

        //instance_ is set to nullptr in cpp file
        static SkillsCata* instance_;

        pros::MotorGroup& motors;
        pros::Rotation& rotation_sensor;
        const int upright_position = 12000; //centidegrees, not sure if should be position or angle, temp value
        const int cata_rpm = 100; // temp value

        int current_rot;
        int last_rot;

        pros::Task cata_task;
        std::string cata_mode = "X";
};

void cata_task_funct();
