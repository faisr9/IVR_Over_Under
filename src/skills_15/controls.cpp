#include "skills_15/controls.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <string>




void controls()
{
  while (1)
  {
    pros::lcd::print(1, "Hello!");

    pros::delay(20);

  }
}