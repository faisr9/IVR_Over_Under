#include "comp_15/cata.h"
// note: velocity in rpm

CompetitionCatapult* CompetitionCatapult::createInstance(pros::MotorGroup& motorgroup, pros::ADIButton& limit_switch) {
    if (!instance_) {
        instance_ = new CompetitionCatapult(motorgroup, limit_switch);
    }
    return instance_;
}

CompetitionCatapult::CompetitionCatapult(pros::MotorGroup& motorgroup, pros::ADIButton& limit_switch) 
    : SubsystemParent("Competition Catapult"), motors(motorgroup), kill_switch(limit_switch), cata_task(cata_task_funct){
    
    /** NOTE: Comment this out once cata code works */
    // pros::lcd::set_text(5, "In constructor");

    cata_mode = "X";
    motors.set_brake_modes(BRAKETYPE_HOLD);
}

CompetitionCatapult* CompetitionCatapult::getInstance() {
    if (instance_ == nullptr) {
        throw std::runtime_error("Attempting to getInstance while instance_ is nullptr");
    }

    return instance_;
}
CompetitionCatapult::~CompetitionCatapult() {
    cata_task.suspend();
    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

void CompetitionCatapult::stop() {
    motors.brake();
}

void CompetitionCatapult::prime() {
    // cata_task.suspend();
    // cata_mode = "P";
    // cata_task.resume();

    // pros::Task prime_task{[=] {
	// 	while (!kill_switch.get_value()) {
    //         motors.move(cata_voltage);
	// 		delay(30);

	// 	}

    //     stop();
	// }}; // lambda function with a task


    while (!kill_switch.get_value()) {
        motors.move(cata_voltage);
		delay(30);
	}

    // stop();
}

//test this
void CompetitionCatapult::cycle() {
    if (!kill_switch.get_value()) {
        prime();
        release();
    } else {
        release();
        prime();
    }
}

void CompetitionCatapult::release() {
    // cata_task.suspend();
    // cata_mode = "R";
    // cata_task.resume();
    while (kill_switch.get_value()) {
        motors.move(cata_voltage);
        delay(30);
    }
    // stop();
}

std::string CompetitionCatapult::get_cata_mode() {
    return cata_mode;
}

void CompetitionCatapult::set_cata_mode(std::string new_cata_mode) {
    cata_task.suspend();
    cata_mode = new_cata_mode;
    cata_task.resume();
}


// Function for the catapult class' task. Don't call this unless you're passing it to a task
void cata_task_funct() {
    // stuff goes here
    CompetitionCatapult* cata_inst = CompetitionCatapult::getInstance();

    while (!cata_inst) {
        pros::delay(3000);
        cata_inst = CompetitionCatapult::getInstance();
    }

    while (1) {
        std::string cata_mode = cata_inst->get_cata_mode();

        if (cata_mode == "X") {
            cata_inst->stop();
        } else if (cata_mode == "PR") {
            cata_inst->prime();
            cata_inst->set_cata_mode("R");
        } else if (cata_mode == "RP") {
            cata_inst->release();
            cata_inst->set_cata_mode("P");
        } else if (cata_mode == "R") {
            cata_inst->release();
        } else if (cata_mode == "P") {
            cata_inst->prime();
        } else {
            pros::lcd::set_text(6, "Invalid cata_mode string!");
            cata_inst->stop();
        }
        pros::delay(30);
    }
}

// void cata_task_funct() {
//     // stuff goes here
//     CompetitionCatapult* cata_inst = CompetitionCatapult::getInstance();

//     if (cata_inst == nullptr) {
//         return;
//     }

//     while (1) {
//         if (cata_mode == "X") {
//             cata_inst->stop();
//         } else if (cata_mode == "PR") {
//             if (!cata_inst->prime()) {
                
//             }
//             if (!kill_switch.get_value()) {
//                 motors.move(cata_voltage);
//             } else {
//                 cata_mode = "R";
//             }
//         } else if (cata_mode == "RP") {
//             if (kill_switch.get_value()) {
//                 motors.move(cata_voltage);
//             } else {
//                 cata_mode = "P";
//             }
//         } else if (cata_mode == "R") {
//             if (kill_switch.get_value()) {
//                 motors.move(cata_voltage);
//             } else {
//                 cata_mode = "X";
//             }
//         } else if (cata_mode == "P") {
//             if (!kill_switch.get_value()) {
//                 motors.move(cata_voltage);
//             } else {
//                 cata_mode = "X";
//             }
//         } else {
//             pros::lcd::set_text(6, "Invalid cata_mode string!");
//             cata_inst->stop();
//         }
//         pros::delay(30);
//     }
// }
