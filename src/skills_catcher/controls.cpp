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

  while (1) {

    // lcd::print(2,"Left middle: %i",(int)left_middle_2.get_actual_velocity());
		// lcd::print(3,"Right middle: %i",(int)right_middle_2.get_actual_velocity());
		// lcd::print(4,"Front left: %i",(int)front_left.get_actual_velocity());
		// lcd::print(5,"Front right: %i",(int)front_right.get_actual_velocity());
		// lcd::print(6,"Back left: %i",(int)back_left.get_actual_velocity());
		// lcd::print(7,"Back right: %i",(int)back_right.get_actual_velocity());

    astdriveCatcher.run();

    pros::delay(20);
  }
}