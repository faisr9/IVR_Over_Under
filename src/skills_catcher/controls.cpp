#include "skills_catcher/controls.h"

void controls()
{
  pros::lcd::print(1, "Hello!");

  pros::Task odom_task{[=] {
		while (1) {
			ast_odom.updatePosition();
			pros::delay(50);
		}
	}};

  double p = 2.7;

  while (1) 
  {
    astdriveCatcher.run();

    if(ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_A))
      catcher_wings.toggle();


    // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
    //   turnToAngleX(astdriveCatcher, ast_odom, 0, 2, false, p, 200);
    // } else if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
    //   turnToAngleX(astdriveCatcher, ast_odom, 180, 2, false, p, 200);
    // }

    // if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
    //   p -= 0.1;
    // } else if (ctrl_master.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)) {
    //   p += 0.1;
    // }

    pros::lcd::set_text(5, "p is " + std::to_string(p));

    // astdriveCatcher.run();

    pros::delay(20);
  }
}