#include "skills_thrower/controls.h"
#include "skills_thrower/skills.h"

void controls()
{

  pros::Task odom_task{[=] {
		while (1) {
			x_drive_odom.updatePosition();
			pros::delay(50);
		}
	}};
  
  pros::lcd::print(1, "Running Controls!");
  // pros::lcd::set_background_color(0, 0, 0); :((((
  while (1)
  {

    if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
      SkillsCata::getInstance()->set_cata_mode(SkillsCata::CataMode::Cycle);
    } 
    
    
    if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_RIGHT)) {
      SkillsCata::getInstance()->move_forward_manual();
    } 
    
    if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)) {
      SkillsCata::getInstance()->stop();
    }

    if (ctrl_master.get_digital(E_CONTROLLER_DIGITAL_R2))
      skills();

    // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
    //   turnToAngleX(xdriveThrower, x_drive_odom, 0, 2, false, p, 200);
    // } else if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
    //   turnToAngleX(xdriveThrower, x_drive_odom, 180, 2, false, p, 200);
    // }

    // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
    //   p -= 0.1;
    // } else if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)) {
    //   p += 0.1;
    // }

    xdriveThrower.run();

    pros::delay(20);
  }
}