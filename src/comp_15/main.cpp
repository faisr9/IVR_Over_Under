#include "main.h"

void clear_rType_warn(void){rtype_warn=0;}
void allow_override(void){rtype_warn=7;lcd::clear();}
void secret_override(void){lcd::register_btn2_cb(allow_override);}

// /* First method to run when program starts */
void initialize() {
	lcd::initialize(); // Used for warning messages

	if(rTypeCheck) {
	if(!usd::is_installed()) // No SD card installed 
		rtype_warn = 3;
	else if(fileExists(COMP_15_CHECKFILE)) // If file exists, check value
	{
		FILE *comp_15_check_r = fopen(COMP_15_CHECKFILE, "r");
		char checkValue[17];
		fread(checkValue, 1, 17, comp_15_check_r);
		fclose(comp_15_check_r);

		std::string checkValueStr(checkValue);
		if(strcmp(checkValueStr.c_str(), COMP_15_CHECKVALUE) != 0)
			rtype_warn = 2;	
	}
	else // If file doesn't exist, prompt warn msg, then create it
		rtype_warn = 1;

	if(rtype_warn==1)
	{
		mismatch_override:
		lcd::print(0, "Robot Type File Missing");
		lcd::print(1, "To confirm and save robot type,"); 
		lcd::print(2, "press middle button");
		lcd::print(3, "Locking up program to prevent");
		lcd::print(4, "damage to robot!");
		lcd::print(6, "Robot Type: Comp 15 Bot");
		lcd::register_btn1_cb(clear_rType_warn);
		while(1)
		{
			Task::delay(1000); // Lock up program
			if(rtype_warn==0)
				break;
		}
		lcd::clear();
		lcd::print(0, "Saving Robot Type...");
		lcd::print(1, "Comp 15 Bot");
		FILE *comp_15_check_w = fopen(COMP_15_CHECKFILE, "w");
		fputs(COMP_15_CHECKVALUE, comp_15_check_w);
		fclose(comp_15_check_w);
		delay(2500);
		// lcd::shutdown();
	}
	else if(rtype_warn==2)
	{
		lcd::print(0, "Robot Type Mismatch");
		lcd::print(1, "Check ./robot_type.mk & rebuild");
		lcd::print(2, "Locking up program to prevent");
		lcd::print(3, "damage to robot!");
		lcd::register_btn0_cb(secret_override);
		while(1)
		{
			Task::delay(1000); // Lock up program
			if(rtype_warn==7)
				goto mismatch_override;
		}
	}
	else if(rtype_warn==3)
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
		// lcd::shutdown();
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
	lcd::clear();
	lcd::print(0,"15 Comp");

	while(1)
		Task::delay(1000);
}
