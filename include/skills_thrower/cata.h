
#pragma once
#include "skills_thrower/devices.h"

class SkillsCata : public SubsystemParent {
    public:
        static SkillsCata* createInstance(pros::MotorGroup& motorgroup, pros::Rotation& rotation_sensor);
        static SkillsCata* getInstance(); // static because need to be able to access without ExampleSubsystem objecct
        ~SkillsCata();
        SkillsCata(const SkillsCata& other) = delete;

        // use move forward manual or stop to do those things (no CataMode required)
        // though catapult will ignore if you if it's doing a cycle
        enum CataMode {
            Idle, // do nothing
            Cycle, // cycle once
        };

        void stop();
        void cycle(bool stop_at_end);

        void move_forward_manual();
        CataMode get_cata_mode();   

        void set_cata_mode(CataMode mode);
        void set_cata_mode_internal(CataMode mode);

    private:
        // reference to passed in motor so it doesn't create a new motor object 
        SkillsCata(pros::MotorGroup& motorgroup, pros::Rotation& rotation_sensor);

        //instance_ is set to nullptr in cpp file
        static SkillsCata* instance_;

        pros::MotorGroup& motors;
        pros::Rotation& rotation_sensor;
        const int upright_position = 12000; //centidegrees, not sure if should be position or angle, temp value
        const int cata_rpm = 100; // temp value

        pros::Task cata_task;
        CataMode cata_mode = CataMode::Idle;
};

void cata_task_funct();
