#pragma once
#include "main.h"
//positive power goes in and negative goes out;
class IntakeClass : public SubsystemParent
{
    public:
        IntakeClass(const IntakeClass& other) = delete;
        ~IntakeClass();
        static IntakeClass* createInstance(pros::Motor& motor);
        static IntakeClass* getInstance();
        void stop();
        void set_power(int power);

    private:
        IntakeClass(pros::Motor& subsystem_motor) : SubsystemParent("Intake"), intake_motor_(subsystem_motor){};
        pros::Motor intake_motor_;
        static IntakeClass* instance_;
};

// TEST_CODE: Throw the bottom things in main
// Controller master(E_CONTROLLER_MASTER);
// 	pros::Motor intakeMotor = pros::Motor(14);
// 	IntakeClass* intakeInstance = IntakeClass::createInstance(intakeMotor);
// 	while (1){
// 		// intakeInstance->set_power(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
// 		if (master.get_digital(E_CONTROLLER_DIGITAL_A)){
// 			intakeInstance->set_power(12000);
// 		}
// 		else if (master.get_digital(E_CONTROLLER_DIGITAL_B)){
// 			intakeInstance->set_power(-12000);
// 		}
// 		pros::delay(50);
// 	}