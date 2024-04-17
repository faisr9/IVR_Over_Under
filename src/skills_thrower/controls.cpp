#include "skills_thrower/controls.h"

void controls()
{
  while (1)
  {
    pros::lcd::print(1, "Running Controls!");

    if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
      SkillsCata::getInstance()->set_cata_mode(SkillsCata::CataMode::Cycle);
    }

    pros::delay(20);
  }
}