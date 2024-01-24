#include "comp_15/comp15_includeList.h"

/* First method to run when program starts */
void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonomous method */
void autonomous() {
}

/* Opcontrol method runs by default (unless connected to comp controller )*/
void opcontrol() {
	autoLogger->pauseAutoLog();

	// cout << "Running Log Tests";
	printf("Running Log Tests\n");
	Logger logBuild("/usd/testlog", false, false, true);
	
	// logBuild.readback();


	logBuild.logStringMessage("Hello World");
    logBuild.logStringMessage("This is cool");
    logBuild.logStringMessage("Rishi is black");
    int x = 5;
    int y = 10;
    double z = 3.14159265358;
    logBuild.logCharMessage("x: %d, y: %d, z: %f", x, y, z);

    int larray[5] = {11235, 2563123, 35645, -4564, -455};
    logBuild.logArray("Array", larray, 5);

    int lvarible1 = 5;
    logBuild.logVarible("varible1", lvarible1);
    double lvarible2 = 2.17856456156485;
    logBuild.logVarible("varible2", lvarible2);
    bool lvarible3 = true;
    logBuild.logVarible("varible3", lvarible3);
    std::string lvarible4 = "World Hello!";
    // logBuild.logVarible("varible4", lvarible4);

    double larray2[5] = {1.1235, 2.563123, 3.5645, -4.564, -4.55};
    logBuild.logArray("Array2", larray2, 5);
    bool larray3[5] = {true, false, true, false, true};
    logBuild.logArray("Array3", larray3, 5);
    std::string larray4[5] = {"Hello", "World", "This", "Is", "Cool"};
    logBuild.logArray("Array4", larray4, 5);

	autoLogger->resumeAutoLog();
	delay(3000);
	autoLogger->pauseAutoLog();

	autoLogger->logStringMessage("Hello World");
    autoLogger->logCharMessage("xw: %d, y: %d, z: %f", 5, 10, 3.14159265358);
    int avalue = 5;
    autoLogger->logVarible("value", avalue);
    double avalue2 = 2.17856456156485;
    autoLogger->logVarible("value2", avalue2);
    bool avalue3 = true;
    autoLogger->logVarible("value3", avalue3);
    std::string avalue4 = "World Hello!";
    autoLogger->logVarible("value4", avalue4);

	autoLogger->resumeAutoLog();
	delay(3000);
	autoLogger->pauseAutoLog();

    int aarray[5] = {11235, 2563123, 35645, -4564, -455};
    autoLogger->logArray("Array", aarray, 5);
    double aarray2[5] = {1.1235, 2.563123, 3.5645, -4.564, -4.55};
    autoLogger->logArray("Array2", aarray2, 5);
    bool aarray3[5] = {true, false, true, false, true};
    autoLogger->logArray("Array3", aarray3, 5);
    std::string aarray4[5] = {"Hello", "World", "This", "Is", "Cool"};
    autoLogger->logArray("Array4", aarray4, 5);

	autoLogger->pauseAutoLog();

	cout << "Done Running Log Tests";
	
	delay(1000);
	std::abort();
}
