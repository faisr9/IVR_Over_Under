#include "comp_15/comp15_includeList.h"

void controls() {
    while(1) {
        pros::Motor intakeMotor = pros::Motor(14);
        IntakeClass* intakeInstance = IntakeClass::createInstance(intakeMotor);
        while (1){
        // intakeInstance->set_power(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
        if (master.get_digital(E_CONTROLLER_DIGITAL_A)){
            intakeInstance->set_power(12000);
        }
        else if (master.get_digital(E_CONTROLLER_DIGITAL_B)){
            intakeInstance->set_power(-12000);
        }
        pros::delay(50);
    }
    }
}