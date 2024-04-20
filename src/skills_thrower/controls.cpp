#include "skills_thrower/controls.h"

void controls()
{

  pros::Task odom_task{[=] {
		while (1) {
			x_drive_odom.updatePosition();
			pros::delay(50);
		}
	}};
  
  pros::lcd::print(1, "Running Controls!");

  while (1)
  {

    // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
    //   SkillsCata::getInstance()->set_cata_mode(SkillsCata::CataMode::Cycle);
    // }

    xdriveThrower.run();

    pros::delay(20);
  }
}