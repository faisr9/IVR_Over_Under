#include "common_code/example_subsystem.h"
#include <sstream>

pros::Motor test_mtr(14);
ExampleSubsystem* example_subsystem_instance = ExampleSubsystem::createInstance(test_mtr);