#include "main.h"

// To relocate to common_code.cpp
static bool fileExists(std::string fileName)
{
	FILE *fileChecker = fopen(fileName.c_str(), "r");
	if(fileChecker == NULL)
	{ fclose(fileChecker); return false; }
	else { fclose(fileChecker); return true; }
}

void clear_rType_warn(){rtype_warn=0;}

// void initialize()
// {
// 	FILE *comp_15_check_w = fopen(COMP_15_CHECKFILE, "w");
// 	fputs(COMP_15_CHECKVALUE, comp_15_check_w);
// 	fclose(comp_15_check_w);
// }

/* First method to run when program starts */
void initialize() {
	lcd::initialize(); // Used for warning messages of robot type

	if(!usd::is_installed())
	{
		lcd::print(0, "SD Card Not Installed!");
		lcd::print(1, "Locking up program to prevent");
		lcd::print(2, "damage to robot!");
		lcd::print(3, "Press CB to Override");
		lcd::register_btn1_cb(clear_rType_warn);
		while(1)
		{
			Task::delay(1000); // Lock up program
			if(rtype_warn==0)
				break;
		}
	}
	else {	/* Robot Config Check */
	if(fileExists(COMP_15_CHECKFILE)) // If file exists, check value
	{
		FILE *comp_15_check_r = fopen(COMP_15_CHECKFILE, "r");
		std::string checkValue;
		fread(&checkValue, sizeof(checkValue), 1, comp_15_check_r);
		fclose(comp_15_check_r);

		if(checkValue != COMP_15_CHECKVALUE) // If value doesn't match, print error
			rtype_warn = 2;
	}
	else // If file doesn't exist, prompt warn msg, then create it
		rtype_warn = 1;

	if(rtype_warn==1)
	{
		lcd::print(0, "Robot Type File Missing");
		lcd::print(1, "To confirm and save robot,"); 
		lcd::print(2, "press middle button");
		lcd::print(3, "Locking up program to prevent");
		lcd::print(4, "damage to robot!");
		lcd::register_btn1_cb(clear_rType_warn);
		while(1)
		{
			Task::delay(1000); // Lock up program
			if(rtype_warn==0)
				break;
		}
		FILE *comp_15_check_w = fopen(COMP_15_CHECKFILE, "w");
		fputs(COMP_15_CHECKVALUE, comp_15_check_w);
		fclose(comp_15_check_w);
		lcd::shutdown();
	}
	else if(rtype_warn==2)
	{
		lcd::print(0, "Robot Type Mismatch");
		lcd::print(1, "Check ./robot_type.mk & rebuild");
		lcd::print(2, "Locking up program to prevent");
		lcd::print(3, "damage to robot!");
		while(1)
		{
			Task::delay(1000); // Lock up program
			// std::abort();
		}
	}}
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	// lcd::print(0,"15 Comp");

	while(1)
		Task::delay(1000);
}
