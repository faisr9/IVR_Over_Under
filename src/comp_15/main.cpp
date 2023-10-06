#include "main.h"

// To relocate to common_code.cpp
static bool fileExists(std::string fileName)
{
	// std::fstream fileChecker;
	// fileChecker.open("s", std::ios::in);
	// bool ex = fileChecker.is_open();
	// fileChecker.close();
	// return ex;

	FILE *fileChecker = fopen(fileName.c_str(), "r");
	if(fileChecker == NULL)
	{ fclose(fileChecker); return false; }
	else
	{ fclose(fileChecker); return true; }
}
void start(){rtype_warn=0;}

/* First method to run when program starts */
void initialize() {
	lcd::initialize(); // Temp until custom GUI

	/* Robot Config Check */
	if(fileExists(COMP_15_CHECKFILE)) // If file exists, check value
	{
		FILE *comp_15_check_r = fopen(COMP_15_CHECKFILE, "r");
		std::string checkValue;
		fread(&checkValue, sizeof(checkValue), 1, comp_15_check_r);
		fclose(comp_15_check_r);

		if(checkValue != COMP_15_CHECKVALUE) // If value doesn't match, print error
			rtype_warn = 2;
	}
	else // If file doesn't exist, prompt msg, then create it
		rtype_warn = 1;

	// Temporary until custom GUI msg
	if(rtype_warn==1)
	{
		lcd::print(0, "Robot Type File Missing");
		lcd::print(1, "To confirm, press middle button");
		lcd::print(2, "Locking up program to prevent damage to robot!");
		lcd::register_btn1_cb(start);
		while(1)
		{
			Task::delay(1000); // Lock up program
			if(rtype_warn==0)
				break;
		}
		FILE *comp_15_check_w = fopen(COMP_15_CHECKFILE, "w");
		fwrite(COMP_15_CHECKVALUE, sizeof(COMP_15_CHECKVALUE), 1, comp_15_check_w);
		fclose(comp_15_check_w);
		lcd::shutdown();
	}
	else if(rtype_warn==2)
	{
		lcd::print(0, "Robot Type Mismatch");
		lcd::print(1, "Check ./robot_Type.mk and rebuild");
		lcd::print(2, "Locking up program to prevent damage to robot!");
		while(1)
		{
			// Task::delay(1000); // Lock up program
			std::abort();
		}
	}
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	lcd::print(0,"15 Comp");

	while(1)
		Task::delay(1000);
}
