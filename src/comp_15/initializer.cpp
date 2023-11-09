#include "comp_15/example_sub_test.h"
#include "comp_15/example_test_two.h"
#include "common_code/example_subsystem.h"
#include <sstream>

pros::Motor test_mtr(14);
ExampleSubsystem* inst = ExampleSubsystem::createInstance(test_mtr);