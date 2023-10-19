#include "common_code/subsystem_parent.h"
#include "api.h"


class ExampleSubsystem : public SubsystemParent {
    public:
        ExampleSubsystem(std::string subsystem_name, pros::Motor& subsystem_motor);

        void stop();
        std::string get_subsystem_name() const;
    private:
        // reference to passed in motor so it doesn't create a new motor object 
        // (which wouldn't be the end of the world but still)
        pros::Motor& subsystem_motor_;

};