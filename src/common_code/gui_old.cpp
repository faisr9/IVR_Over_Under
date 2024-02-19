/* Features to add / Things to change
---------------------------------------
- Runner method to control the gui
    - This method will be the one that runs the GUI and sets it up
    - Refresh loop to allow updates or seperate thread of very low priority that
        updates the changing label values
 - Finish Screen updater
 - move updates to updateGui
 - Finish Port changer screen
 - Final code clean up
 - Screen saver timer trigger (Will default to non glitch raze)
 - Add a single varible check that will enable/disable the gui incase of unforseen bug
    - This will disable the a large operational section of the gui, but still allow
    certain options to continue running like the auton selector
 - Add a button for toggling the motor balance should loss of a drive motor occur
 - Make SD card button red if missing
 - Add gyro to contrustor to allow for gyro reading
 - Combine the logo c files into one file
 - Add update cycle to the device checker
 - In SetAuton make the field map change when the auton is skills
 
/// On Hold ///
 - For PortsList, the deviceList in devices.cpp needs to be extern imported
    for the intilization of portslist based on current init.
 - Add while loop control to settings for imu info
 - Finish Odom map screen
 - Device check auto update with refresh loop
 - Have animation for robud icon working
*/

#include "main.h"
#include <string>
#include <fstream>

// Avoids null conversion issues in complietime //
#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif
///////////////////////////////////////////////////

#define FLYWHEEL_FILE "/usd/flywheel.txt" // Move to Header file
#define INTAKE_FILE "/usd/intake.txt" // Move to Header file
#define DRIVER_PROFILE_FILE "/usd/driver_profile.txt" // Move to Header file

brain_gui BrainGUI; // GLobal instance of the GUI class

/*-------GUI Class static forward declares-------*/
    int brain_gui::currentScreen;
    int brain_gui::lastScreen;
    bool brain_gui::disable;
    int brain_gui::autonColor;
    int brain_gui::autonType;
    int brain_gui::inertialStat;
    // float brain_gui::sliderPos;
    char brain_gui::sliderPos_fly_str[14];
    char brain_gui::sliderPos_intake_str[14];
    bool brain_gui::runRaze;
    bool brain_gui::glitch;
    pros::screen_touch_status_s_t brain_gui::lastClick;
    float brain_gui::delayCountOne;
    // float brain_gui::delayCountTwo;
    bool brain_gui::isLogo;
    uint8_t brain_gui::id;
    bool brain_gui::updated;
    bool brain_gui::isDeviceSelected;
    bool brain_gui::isV5Port;
    bool brain_gui::isMotor;
    bool brain_gui::isRotation;
    short int brain_gui::deviceSelected;
    bool brain_gui::deviceConnectionError;
    chrono::high_resolution_clock::time_point brain_gui::guiLastUpdate;
    chrono::high_resolution_clock::time_point brain_gui::guiCurrentTime;
    int brain_gui::update_TimeEslaped;
/*===============================================*/

/**
* Constructor for the brainGUI setting default values and initization of varibles
*/
brain_gui::brain_gui() {
    brain_gui::currentScreen = 0;
    brain_gui::lastScreen = MAINMENU;
    brain_gui::disable = false;
    brain_gui::autonColor = 0;
    brain_gui::autonType = 0;
    brain_gui::inertialStat = 2;
    // brain_gui::sliderPos = 6.0;
    strcpy(brain_gui::sliderPos_fly_str, "");
    strcpy(brain_gui::sliderPos_intake_str, "");
    brain_gui::runRaze = false;
    brain_gui::glitch = false;
    brain_gui::delayCountOne = 0.0;
    brain_gui::isLogo = false;
    brain_gui::updated = false;
    brain_gui::isDeviceSelected = false;
    brain_gui::isV5Port = false;
    brain_gui::isMotor = false;
    brain_gui::isRotation = false;
    brain_gui::deviceSelected = 0;
    brain_gui::deviceConnectionError = false;
    brain_gui::update_TimeEslaped = 0;
}

struct deviceStatus
{
    std::string name;
    std::string stat; // Connected, Disconnected, overTemp
};

deviceStatus deviceStrings[3][9] =
{
    {{"FRTM",""},{"FRBM",""},{"BRM",""},{"FLTM",""},{"FLBM",""},{"BLM",""},{"CataR",""},{"CataL",""},{"Intake",""}},
    {{"IMU",""},{"Dist",""}},
    {{"Limit",""}, {"x_Enc",""}, {"y_Enc",""}, {"FlBr",""},{"Wing",""}}
};

const string labelStrings[21] = 
{
    // Autons //
    "Unknown",
    "No Auton",
    "No Auton",
    "No Auton",
    "No Auton",
    "No Auton",
    // Ports List and Device Change Rollers
    "FRTM\nFRBM\nBRM\nFLTM\nFLBM\nBLM\nCataL\nCataR\nIntake",
    "Gyro\nDist",
    "FloorBrake\nPneumatics",
    // Manual Functions
    "empty",
    "empty",
    "empty",
    "empty",
    "empty",
    "empty",
    "empty",
    "empty",
    "empty",
    "empty",
    "empty",
    "empty"
};

// Will move
static bool fileExists(std::string fileName)
{
    bool ex;
    std::fstream fileChecker;
    fileChecker.open(fileName, std::ios::in);
    ex = fileChecker.is_open();
    fileChecker.close();
    return ex;
}

/**
* 0) Brain batt in MAINMENU
* 1) USD installed in MAINMENU
* 2) IMU info in settings
* 3) Manual Auton Stat
* 4) Volts slider in settings
* 5) Current Pos in ODOM
* 6) Current Heading in ODOM
* 7) Motors Ports List
* 8) V5 Device Ports List
* 9) Other Devices Ports List
* 10) Robot type status
*/
string changingLabel[11];

/** Method to create buttons, and simplify code lengths for repeated commands.
 * @param parent lv object parent
 * @param x x-pos of button where x <= SCREEN_WIDTH_MAX
 * @param y y-pos of button where y <= SCREEN_HEIGHT_MAX
 * @param width width of the button
 * @param height height of the button
 * @param style_pressed style of the btn when pressed
 * @param style_released style of the btn when released
 * @param actionTrig the type of button interaction that will trigger the @param buttonAction
 * @param btnAction the method to call upon an interaction of the button
 * @param id unique id number for button
 * @param title label to display on button
 * 
 * @return returns an initialized lv_btn object
 */
lv_obj_t *brain_gui::createBtn(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
    lv_style_t *style_pressed, lv_style_t *style_released, lv_btn_action_t actionTrig, lv_action_t btnAction, int id, const char *title)
{
    lv_obj_t * btn = lv_btn_create(parent, NULL);
    lv_obj_set_pos(btn, x, y);
    
    lv_obj_set_size(btn, width, height);
    lv_obj_set_free_num(btn, id);
    lv_btn_set_style(btn, LV_BTN_STYLE_PR, style_pressed);
    lv_btn_set_style(btn, LV_BTN_STYLE_REL, style_released);
    // if(actionTrig != NULL && btnAction != NULL)
        lv_btn_set_action(btn, actionTrig, btnAction);

    lv_obj_t * label = lv_label_create(btn, NULL);
    lv_label_set_text(label, title);
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);

    return btn;
}

/** Method to create labels, and simplify code lengths for repeated commands.
 * @param parent lv object parent
 * @param x x-pos of button where x <= SCREEN_WIDTH_MAX
 * @param y y-pos of button where y <= SCREEN_HEIGHT_MAX
 * @param width width of the button
 * @param height height of the button
 * @param style the style of the label
 * @param title label to display on button
 * 
 * @return returns an initialized lv_label object
 */
lv_obj_t *brain_gui::createLabel(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
    lv_style_t *style, const char * title)
{
    lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_size(label, width, height);
    lv_label_set_text(label, title);
    lv_label_set_style(label, style);
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0); // 153 77
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, x-label->coords.x1, y-label->coords.y1);

    return label;
}

/** Method to initialize the robotIcon used for the odom map and auton selctor field */
void brain_gui::robotIcon_init()
{
    // lv_style_copy(&robudIconStyle, &lv_style_plain);
    // robudIconStyle.body.radius = LV_RADIUS_CIRCLE;
    // robudIconStyle.body.main_color = LV_COLOR_MAKE(255, 0, 255);
    // robudIconStyle.body.grad_color = LV_COLOR_MAKE(255, 0, 255);
    // robudIconStyle.body.border.color = LV_COLOR_WHITE;
    // robudIconStyle.body.border.width = 3;

    // lv_style_copy(&robudLineStyle, &lv_style_plain);
    // robudLineStyle.line.color = LV_COLOR_MAKE(255, 0, 255);
    // robudLineStyle.line.width = 4;

    // robudIcon = lv_led_create(lv_scr_act(), NULL);
    // lv_led_on(robudIcon);
    // lv_obj_set_style(robudIcon, &robudIconStyle);
    // lv_obj_set_size(robudIcon, 15, 15);
    // robudIcon->hidden = true;

    // robudLine = lv_line_create(lv_scr_act(), NULL);
    // lv_obj_set_style(robudLine, &robudLineStyle);
}

/** Method to update the hidden parameter of the robotIcon
 * @param isHidden true to hide icon, false to show icon
 */
void brain_gui::robotIcon_updateHidden(bool isHidden)
{
    // robudIcon->hidden = isHidden;
    // robudLine->hidden = isHidden;
}

/** Method to update the robotIcon display
 * @param xPos xPos of the robot on the field
 * @param yPos yPos of the robot on the field
 * @param heading the heading ot the robot from [o-360)
 */
void brain_gui::robotIcon_update(int xPos, int yPos, float heading)
{
    // lv_obj_set_pos(robudIcon, xPos, yPos);
    //                                         // Round instead of int16 cast
    // // coords from obj have to be double cast
    // inputX = 10+(int)(15*cos(heading));
    // inputY = 10+(int)(15*sin(heading));
    // rbudHeading[0] = {inputX , inputY};
    // inputX = 10+(int)(25*cos(heading));
    // inputY = 10+(int)(25*sin(heading));
    // rbudHeading[1] = {inputX, inputY};

    // robotIcon_updateHidden(true);
    // lv_line_set_points(robudLine, rbudHeading, 2);
}

/* Deletes previous object to clear screen and clear ram buffer */
void brain_gui::lastScreen_del(int screen)
{
    switch(screen)
    {
        case(LOGOSCREEN): // 0
            lv_obj_del(illiniLogo);
            lv_obj_del(returnMainMenuBtn);
            break;
        case(MAINMENU): // 1
            lv_obj_del(illiniLogoBtn);
            lv_obj_del(setAutonBtn);
            lv_obj_del(settingBtn);
            lv_obj_del(deviceCheckBtn);
            lv_obj_del(PortChangerBtn);
            lv_obj_del(manualCtrlBtn);
            lv_obj_del(odomMapBtn);
            lv_obj_del(batteryInfo);
            lv_obj_del(sdcard);
            lv_obj_del(errorBox);
            lv_obj_del(robotType);
            break;
        case(TEAMINFO): // 2
            lv_obj_del(illiniLogo);
            lv_obj_del(returnMainMenuBtn);
            break;
        case(SETAUTON):  // 3
            lv_obj_del(chooseLable);
            lv_obj_del(fieldMap);
            lv_obj_del(matchAuton);
            lv_obj_del(skills);
            lv_obj_del(autonOne);
            lv_obj_del(autonTwo);
            lv_obj_del(autonThree);
            lv_obj_del(autonFour);
            lv_obj_del(autonFive);
            lv_obj_del(autonSix);
            lv_obj_del(returnMainMenuBtn);
            if(autonColor == 1 || autonColor == 3)
            {
                // lv_obj_del(blueside_line);
                // lv_obj_del(redside_line);
                lv_obj_del(skills_line);
            }
            /// @note auton-lines self delete ///
            break;
        case(SETTINGS): // 4
            lv_obj_del(inertialCaliBtn);
            lv_obj_del(gyroInfo);
            lv_obj_del(manualAutonBtn);
            lv_obj_del(manualAutonStateLabel);
            lv_obj_del(razeBtn);
            lv_obj_del(driverProfileDD);
            // lv_obj_del(flywheelSpeedBtn);
            // lv_obj_del(flywheelSpeedLabel);
            // lv_obj_del(flywheelSpeed);
            // lv_obj_del(intakeSpeedBtn);
            // lv_obj_del(intakeSpeedLabel);
            // lv_obj_del(intakeSpeed);
            lv_obj_del(returnMainMenuBtn);
            break;
        case(RAZE_SS): // 5
            lv_obj_del(frameObj);
            lv_obj_del(razeImg);
            break;
        case(DEVICECHECK): // 6
            lv_obj_del(frtm_btn);
            lv_obj_del(frbm_btn);
            lv_obj_del(brm_btn);
            lv_obj_del(fltm_btn);
            lv_obj_del(flbm_btn);
            lv_obj_del(blm_btn);
            lv_obj_del(intake_btn);
            lv_obj_del(cataR_btn);
            lv_obj_del(cataL_btn);
            lv_obj_del(imu_btn);
            lv_obj_del(distance_btn);
            lv_obj_del(limit_btn);
            lv_obj_del(x_encoder_btn);
            lv_obj_del(y_encoder_btn);
            lv_obj_del(fb_pneu_btn);
            lv_obj_del(wings_pneu_btn);
            // lv_obj_del(motorBtnTitle);
            // lv_obj_del(sensorBtnTitle);
            // lv_obj_del(ADIBtnTitle);
            // lv_obj_del(dc_motorList);
            // lv_obj_del(sensorList);
            // lv_obj_del(ADIList);
            lv_obj_del(returnMainMenuBtn);
            break;
        case(PORTCHANGE): // 7
            lv_obj_del(deviceRol);
            lv_obj_del(V5_portRol);
            lv_obj_del(confirmChangeBtn);
            lv_obj_del(resetDefBtn);
            lv_obj_del(deviceCustomTitle);
            lv_obj_del(reverseSw);
            lv_obj_del(reverseSwTitle);
            lv_obj_del(gearSetSw_06);
            lv_obj_del(gearSetSw_18);
            lv_obj_del(gearSetSw_32);
            lv_obj_del(gearSetSwTitle_06);
            lv_obj_del(gearSetSwTitle_18);
            lv_obj_del(gearSetSwTitle_32);
            lv_obj_del(returnMainMenuBtn);
            break;
        case(DISABLESCREEN): // 8
            // lv_obj_del(disableMsgImg);
            break;
        case(MANUALSCREEN): // 9
            lv_obj_del(func1);
            lv_obj_del(func2);
            lv_obj_del(func3);
            lv_obj_del(func4);
            lv_obj_del(func5);
            lv_obj_del(func6);
            lv_obj_del(func7);
            lv_obj_del(func8);
            lv_obj_del(func9);
            lv_obj_del(func10);
            lv_obj_del(func11);
            lv_obj_del(func12);
            lv_obj_del(returnMainMenuBtn);
            break;
        case(ODOMMAP): // 10
            lv_obj_del(currPos);
            lv_obj_del(heading);
            break;
        case(PORTSLIST): // 11
            lv_obj_del(portslistTitle);
            lv_obj_del(motorsTitle);
            lv_obj_del(v5DeviceTitle);
            lv_obj_del(threeWireTitle);
            lv_obj_del(pl_motorList);
            lv_obj_del(v5DeviceList);
            lv_obj_del(threeWireList);
            break;


    }
}

/** Method to update gui based on button pressed and the current screen
 * @param btn the button object that was pressed
 *
 * @return LV_RES_OK if success
 */
lv_res_t brain_gui::updateGUI(lv_obj_t *btn)
{
    id = lv_obj_get_free_num(btn);
    guiLastUpdate = chrono::high_resolution_clock::now();

    switch(currentScreen)
    {
        case(LOGOSCREEN): // 0
            break;
        case(MAINMENU): // 1
            if(id == MM_BTN_ILLINILOGO) {currentScreen = 2;  printf("Illini Logo Btn\n");}
            else if(id == MM_BTN_SETAUTON) {currentScreen = 3; printf("SetAuton Btn\n");}
            else if(id == MM_BTN_SETTING) {currentScreen = 4; printf("Settings btn\n");}
            else if(id == MM_BTN_DEVICECHECK) {currentScreen = 6; printf("deivce check btn\n");}
            else if(id == MM_BTN_PORTCHANGER) {currentScreen = 7; printf("port changer btn\n");}
            else if(id == MM_BTN_MANUALCTRL) {currentScreen = 9; printf("manual ctrl btn\n");}
            else if(id == MM_BTN_ODOMMAP) {/*currentScreen = 10;*/ printf("odom map btn\n");}
            break;
        case(TEAMINFO): // 2
            break;
        case(SETAUTON):  // 3
            switch(id)
            {
                case(SA_BTN_MATCHAUTON):
                    autonColor = 1;
                    autonType = 1;
                    printf("Match Auton\n");
                    break;
                case(SA_BTN_SKILLS):
                    autonColor = 3;
                    autonType = 0;
                    printf("Skills Auton\n");
                    break;
                case(SA_BTN_AUTONONE):
                    autonType = 1;
                    printf("Auton 1\n");
                    break;
                case(SA_BTN_AUTONTWO):
                    autonType = 2;
                    printf("Auton 2\n");
                    break;
                case(SA_BTN_AUTONTHREE):
                    autonType = 3;
                    printf("Auton 3\n");
                    break;
                case(SA_BTN_AUTONFOUR):
                    autonType = 4;
                    printf("Auton 4\n");
                    break;
                case(SA_BTN_AUTONFIVE):
                    autonType = 5;
                    printf("Auton 5\n");
                    break;
                case(SA_BTN_AUTONSIX):
                    autonType = 6;
                    printf("Auton 6\n");
                    break;
                default:
                    break;
            }

            if(autonColor == 3)
                autonType = 0;
            else if(autonColor == 0 && autonType != 0)
                autonColor = 1;
            break;
        case(SETTINGS): // 4
            if(id == S_BTN_INERTIALCALI) 
            {
                Gyro.reset(true); // Blocks program until calibrated
                printf("Calibrate IMU\n");
            }
            else if(id == S_BTN_MANUALAUTON) {/*Manually trigger auton*/ printf("Maunal Auton\n");}
            else if(id == S_BTN_MINESTRONE) {currentScreen = 0; printf("Mine Strone\n");}
            else if(id == S_BTN_RAZE) {currentScreen = 5; printf("razeing\n");}
            break;
        case(RAZE_SS): // 5
            break;
        case(DEVICECHECK): // 6
            break;
        case(PORTCHANGE): // 7
            if(id == PC_BTN_CONFIRMCHANGEBTN && !isDeviceSelected)
            {
                isDeviceSelected = true;
                if(lv_roller_get_selected(deviceRol) < 8)
                {
                    brain_gui::isMotor = true;
                    brain_gui::isRotation = false;
                    brain_gui::isV5Port = true;
                }
                else if(lv_roller_get_selected(deviceRol) >= 8 && lv_roller_get_selected(deviceRol) < 12)
                {
                    brain_gui::isMotor = false;
                    brain_gui::isRotation = false;
                    brain_gui::isV5Port = true;
                }
                else if(lv_roller_get_selected(deviceRol) >= 12 && lv_roller_get_selected(deviceRol) < 15)
                {
                    brain_gui::isMotor = false;
                    brain_gui::isRotation = true;
                    brain_gui::isV5Port = true;
                }
                else if(lv_roller_get_selected(deviceRol) >= 15)
                {
                    brain_gui::isMotor = false;
                    brain_gui::isRotation = false;
                    brain_gui::isV5Port = false;
                }
            }
            else if(id == PC_BTN_CONFIRMCHANGEBTN && isDeviceSelected)
            {
                currentScreen = DISABLESCREEN;
                // pros::delay(2000);
            }
            break;
        case(DISABLESCREEN): // 8
            break;
        case(MANUALSCREEN): // 9
            break;
        case(ODOMMAP): // 10
            break;
        case(PORTSLIST): // 11
             break;
        default: 
            break;

    }
    // Check for common objects //
    if(id == CO_BTN_MAINMENU) {currentScreen = 1; printf("Return pressed\n");}

    printf("deleting screen %d\n", lastScreen); // To write to log file
    lastScreen_del(lastScreen);
    if(lastScreen == RAZE_SS) {pros::delay(2000);}
    lastScreen = currentScreen;

    runMainGUI();

    return LV_RES_OK;
}

/** Method to update labels on screen based on button pressed and the current screen
 * @param btn the button object that was pressed
 * @note Labels will update every second to save reasources. Odom will update instaneously
 *          Will run with the odom loop/thread
 *
 * @return LV_RES_OK if success
 */
void brain_gui::updateLabel()
{
    pros::c::imu_accel_s_t gyroAccel;
    if(Gyro.get_status() != ENODEV)
        gyroAccel = Gyro.get_accel();

    changingLabel[MM_BTNLABEL_BATTERYINFO-10] = "Batt; " + std::to_string((int)pros::battery::get_capacity()) + "%";

    if(pros::c::usd_is_installed())
        changingLabel[MM_BTNLABEL_SDCARD-10] = "SDCard; Y";
    else
        changingLabel[MM_BTNLABEL_SDCARD-10] = "SDCard; N";

    changingLabel[MM_BTNLABEL_ROBOTTYPE-2] = "comp_15"; // Need to make this dynamic for all 4 robot types

    if(Gyro.get_status() != ENODEV)
        changingLabel[S_LABEL_GYROINFO-32] = "Head; " + std::to_string(Gyro.get_heading()) + " deg\nRot; " + std::to_string(Gyro.get_rotation()) + " deg\nX-Accel; " + std::to_string(gyroAccel.x) + " g\nZ-Accel; " + std::to_string(gyroAccel.z) + " g";

    if(autonColor != 0 && autonType != 0)
        changingLabel[S_LABEL_MANUALAUTONSTATE-34] = "Color; " + std::to_string(autonColor) + "\nType; " + std::to_string(autonType);
    else
        changingLabel[S_LABEL_MANUALAUTONSTATE-34] = "Color; " + std::to_string(autonColor) + "\nType; " + std::to_string(autonType);

    // changingLabel[OM_LABEL_CURRPOS-87] = "Current Pos; (" + OdomState.getXPos() + "[x], " + OdomState.getYPos() + "[y])";
    // changingLabel[OM_LABEL_HEADING-87] = "Current Heading; \n(" + OdomState.getRotation() + "[R], " + OdomState.getHeading() + "[H])";
    changingLabel[OM_LABEL_CURRPOS-85] = "Current Pos; (XXX[x], YYY[y])";
    changingLabel[OM_LABEL_HEADING-87] = "Current Heading; \n(RRR[R], HHH[H])";
}

// /** Method to update label that displays slider value for flywheel speed in settings
//  * @note This method will be seperate of the updateLabel method since its update
//  *          frequency is different than the rest of the labels
//  */
// lv_res_t brain_gui::flySpeedUpdate(lv_obj_t *Inp)
// {
//     if(startUpInit)
//         startUpInit = false;
//     else
//         sliderPos_fly = lv_slider_get_value(flywheelSpeed)/4.0;

//     std::sprintf(sliderPos_fly_str, "F: %2.2f volts", sliderPos_fly);
//     lv_label_set_text(flywheelSpeedLabel, sliderPos_fly_str);

//     // Saves updated value //
//     FILE *flywheel_write_loop = fopen(FLYWHEEL_FILE, "wb");
//     if(flywheel_write_loop)
//     {
//         printf("GUI:Flywheel -> New Value (%2.2f) Saved\n", sliderPos_fly);                     // <- To write to log file
//         fwrite(&sliderPos_fly, sizeof(sliderPos_fly), 1, flywheel_write_loop);
//     }
//     fclose(flywheel_write_loop);

//     return LV_RES_OK;   
// }

// /** Method to update label that displays slider value for intkae speed in settings
//  * @note This method will be seperate of the updateLabel method since its update
//  *          frequency is different than the rest of the labels
//  */
// lv_res_t brain_gui::intakeSpeedUpdate(lv_obj_t *Inp)
// {
//     if(startUpInit)
//         startUpInit = false;
//     else
//         sliderPos_intake = lv_slider_get_value(intakeSpeed)/4.0;

//     std::sprintf(sliderPos_intake_str, "I: %2.2f volts", sliderPos_intake);
//     lv_label_set_text(intakeSpeedLabel, sliderPos_intake_str);

//     // Saves updated value //
//     FILE *intake_write_loop = fopen(INTAKE_FILE, "wb");
//     if(intake_write_loop)
//     {
//         printf("GUI:Intake -> New Value (%2.2f) Saved\n", sliderPos_intake);                     // <- To write to log file
//         fwrite(&sliderPos_intake, sizeof(sliderPos_intake), 1, intake_write_loop);
//     }
//     fclose(intake_write_loop);

//     return LV_RES_OK;   
// }

/**
 * Method to save the last selected drive profile to the SDCard
*/
lv_res_t brain_gui::saveLastDriveProfile(lv_obj_t *btn)
{
    FILE *profile_write = fopen(DRIVER_PROFILE_FILE, "wb");
    driverProfile = lv_ddlist_get_selected(btn);
    // // char *driverProfileStr;
    // // lv_ddlist_get_selected_str(driverProfileDD, driverProfileStr);

    if(profile_write)
    {
        printf("GUI:Driver Profile -> %d aka | Saved\n", driverProfile); // <- To write to log file
        fwrite(&driverProfile, sizeof(driverProfile), 1, profile_write);
    }
    fclose(profile_write);

    return LV_RES_OK;
}

/** Method to set style for devices in DEVICECHECK menu
 * @param btn the btn/motor to check status
 */
void brain_gui::deviceCheck_motorStyle_update(lv_obj_t *btn)
{
    id = lv_obj_get_free_num(btn);
    nameTemp.clear();
    switch(id)
    {
        case(DC_BTNLABEL_FRTM):
            if(FRTM.get_faults() == 0) {
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);
                if(FRTM.is_over_temp()) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &hot);}
            }
            else
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);
            break;
        case(DC_BTNLABEL_FRBM):
            if(FRBM.get_faults() == 0) {
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);
                if(FRBM.is_over_temp()) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &hot);}
            }
            else
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);
            break;
        case(DC_BTNLABEL_BRM):
            if(BRM.get_faults() == 0) {
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);
                if(BRM.is_over_temp()) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &hot);}
            }
            else
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);
            break;
        case(DC_BTNLABEL_FLTM):
            if(FLTM.get_faults() == 0) {
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);
                if(FLTM.is_over_temp()) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &hot);}
            }
            else
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);
            break;
        case(DC_BTNLABEL_FLBM):
            if(FLBM.get_faults() == 0) {
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);
                if(FLBM.is_over_temp()) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &hot);}
            }
            else
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);
            break;
        case(DC_BTNLABEL_BLM):
            if(BLM.get_faults() == 0) {
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);
                if(BLM.is_over_temp()) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &hot);}
            }
            else
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);
            break;
        case(DC_BTNLABEL_CATAR):
            if(CataR.get_faults() == 0) {
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);
                if(CataR.is_over_temp()) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &hot);}
            }
            else
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);
            break;
        case(DC_BTNLABEL_CATAL):
            if(CataL.get_faults() == 0) {
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);
                if(CataL.is_over_temp()) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &hot);}
            }
            else
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);
            break;
        case(DC_BTNLABEL_INTAKE):
            if(Intake.get_faults() == 0) {
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);
                if(Intake.is_over_temp()) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &hot);}
            }
            else
                lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);
            break;
        case(DC_BTNLABEL_IMU+9):
            if(Gyro.get_heading() == ENODEV) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);}
            else {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);}
            break;
        case(DC_BTNLABEL_DISTANCE+9):
            if(WDistance.get() == ENODEV) {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &online);}
            else {lv_btn_set_style(btn, LV_BTN_STYLE_REL, &offline);}
            break;
        case(DC_BTNLABEL_LIMIT+11):
            lv_btn_set_style(btn, LV_BTN_STYLE_REL, &unknown);
            break;
        case(DC_BTNLABEL_XENCODER+11):
            lv_btn_set_style(btn, LV_BTN_STYLE_REL, &unknown);
            break;
        case(DC_BTNLABEL_YENCODER+11):
            lv_btn_set_style(btn, LV_BTN_STYLE_REL, &unknown);
            break;
        case(DC_BTNLABEL_FB_PNEU+11):
            lv_btn_set_style(btn, LV_BTN_STYLE_REL, &unknown);
            break;
        case(DC_BTNLABEL_WINGS_PNEU+11):
            lv_btn_set_style(btn, LV_BTN_STYLE_REL, &unknown);
            break;
        default:
            break;
    }
}

/**
* IMPORTANT: Integrated in the updateScreen method
*/
lv_res_t brain_gui::portScreen_roller_update(lv_obj_t *inpObj)
{
    id = lv_obj_get_free_num(inpObj);

    if(id==PC_ROLLER_DEVICEROL) {deviceSelected = (int)lv_roller_get_selected(deviceRol);}
    printf("Device Val; %d\n", deviceSelected);

    return LV_RES_OK;
}

lv_res_t brain_gui::manualScreenFunction(lv_obj_t *btn)
{
    id = lv_obj_get_free_num(btn);
    if(id == MS_BTN_FUNC1) {/*Function Here*/ printf("BTN_FUNC1\n");}
    else if(id == MS_BTN_FUNC2) {/*Function Here*/ printf("BTN_FUNC2\n");}
    else if(id == MS_BTN_FUNC3) {/*Function Here*/ printf("BTN_FUNC3\n");}
    else if(id == MS_BTN_FUNC4) {/*Function Here*/ printf("BTN_FUNC4\n");}
    else if(id == MS_BTN_FUNC5) {/*Function Here*/ printf("BTN_FUNC5\n");}
    else if(id == MS_BTN_FUNC6) {/*Function Here*/ printf("BTN_FUNC6\n");}
    else if(id == MS_BTN_FUNC7) {/*Function Here*/ printf("BTN_FUNC7\n");}
    else if(id == MS_BTN_FUNC8) {/*Function Here*/ printf("BTN_FUNC8\n");}
    else if(id == MS_BTN_FUNC9) {/*Function Here*/ printf("BTN_FUNC9\n");}
    else if(id == MS_BTN_FUNC10) {/*Function Here*/ printf("TN_FUNC10\n");}
    else if(id == MS_BTN_FUNC11) {/*Function Here*/ printf("TN_FUNC11\n");}
    else if(id == MS_BTN_FUNC12) {/*Function Here*/ printf("TN_FUNC12\n");}
    return LV_RES_OK;
}

/** Main initialization method for the gui. Creates styles and intilizes common objects 
* IMPORTANT: To be called in the initialize(void) function in @file main.cpp
*/
void brain_gui::lvglGUIInit()
{
    lastScreen = MAINMENU;
    // Global Styles //
        lv_style_copy(&plain_black, &lv_style_plain);
        plain_black.body.main_color = LV_COLOR_BLACK;
        plain_black.body.grad_color = LV_COLOR_BLACK;
        plain_black.body.radius = 0;

        lv_style_copy(&brainBTNrelease, &lv_style_plain);
        brainBTNrelease.body.main_color = LV_COLOR_MAKE(72, 139, 247);
        brainBTNrelease.body.grad_color = LV_COLOR_MAKE(72, 139, 247);
        brainBTNrelease.body.radius = 1;
        brainBTNrelease.text.color = LV_COLOR_MAKE(255, 255, 255);
        lv_style_copy(&brainBTNpressed, &lv_style_plain);
        brainBTNpressed.body.main_color = LV_COLOR_MAKE(131, 196, 252);
        brainBTNpressed.body.grad_color = LV_COLOR_MAKE(131, 196, 252);
        brainBTNpressed.body.radius = 15;
        brainBTNpressed.text.color = LV_COLOR_MAKE(255, 255, 255);

        lv_style_copy(&brainBTNrelease_og, &lv_style_plain);
        brainBTNrelease_og.body.main_color = LV_COLOR_BLACK;
        brainBTNrelease_og.body.grad_color = LV_COLOR_BLACK;
        brainBTNrelease_og.body.border.width = 3;
        brainBTNrelease_og.body.border.color = LV_COLOR_MAKE(255, 85, 46);
        brainBTNrelease_og.body.radius = 1;
        brainBTNrelease_og.text.color = LV_COLOR_MAKE(255, 85, 46);
        lv_style_copy(&brainBTNpressed_og, &lv_style_plain);
        brainBTNpressed_og.body.main_color = LV_COLOR_MAKE(60, 60, 60);
        brainBTNpressed_og.body.grad_color = LV_COLOR_MAKE(60, 60, 60);
        brainBTNpressed_og.body.border.width = 3;
        brainBTNpressed_og.body.border.color = LV_COLOR_MAKE(255, 100, 66);
        brainBTNpressed_og.body.radius = 15;
        brainBTNpressed_og.text.color = LV_COLOR_MAKE(255, 100, 66);

        lv_style_copy(&brainBTNrelease_bl, &lv_style_plain);
        brainBTNrelease_bl.body.main_color = LV_COLOR_BLACK;
        brainBTNrelease_bl.body.grad_color = LV_COLOR_BLACK;
        brainBTNrelease_bl.body.border.width = 3;
        brainBTNrelease_bl.body.border.color = LV_COLOR_MAKE(0, 112, 192);
        brainBTNrelease_bl.body.radius = 1;
        brainBTNrelease_bl.text.color = LV_COLOR_MAKE(0, 112, 192);
        lv_style_copy(&brainBTNpressed_bl, &lv_style_plain);
        brainBTNpressed_bl.body.main_color = LV_COLOR_MAKE(60, 60, 60);
        brainBTNpressed_bl.body.grad_color = LV_COLOR_MAKE(60, 60, 60);
        brainBTNpressed_bl.body.border.width = 3;
        brainBTNpressed_bl.body.border.color = LV_COLOR_MAKE(60, 172, 252);
        brainBTNpressed_bl.body.radius = 15;
        brainBTNpressed_bl.text.color = LV_COLOR_MAKE(60, 172, 252);

        lv_style_copy(&topInfo, &lv_style_plain);
        topInfo.body.main_color = LV_COLOR_BLACK;
        topInfo.body.grad_color = LV_COLOR_BLACK;
        topInfo.body.border.width = 2;
        topInfo.body.border.color = LV_COLOR_WHITE;
        topInfo.body.radius = 2;
        topInfo.text.color = LV_COLOR_WHITE;
        lv_style_copy(&topInfo2, &topInfo);
        topInfo2.body.border.color = LV_COLOR_BLACK;

        lv_style_copy(&errorBoxStyle, &lv_style_plain);
        errorBoxStyle.body.main_color = LV_COLOR_RED;
        errorBoxStyle.body.grad_color = LV_COLOR_RED;
        errorBoxStyle.body.radius = 2;
        errorBoxStyle.text.color = LV_COLOR_WHITE;
    ////////////////////////////
    
    // SETAUTON Menu Styles //`
        lv_style_copy(&matchAutonStyle, &lv_style_plain);
        matchAutonStyle.body.main_color = LV_COLOR_BLUE;
        matchAutonStyle.body.grad_color = LV_COLOR_RED;
        matchAutonStyle.body.radius = 1;
        matchAutonStyle.text.color = LV_COLOR_WHITE;
        lv_style_copy(&matchAutonStyle_tog, &matchAutonStyle);
        matchAutonStyle_tog.body.main_color = LV_COLOR_MAKE(100, 0, 0);
        matchAutonStyle_tog.body.grad_color = LV_COLOR_MAKE(0, 0, 100);
        matchAutonStyle_tog.body.radius = 10;

        lv_style_copy(&skillsStyle, &matchAutonStyle);
        skillsStyle.body.main_color = LV_COLOR_GREEN;
        skillsStyle.body.grad_color = LV_COLOR_GREEN;
        lv_style_copy(&skillsStyle_tog, &matchAutonStyle_tog);
        skillsStyle_tog.body.main_color = LV_COLOR_MAKE(0, 100, 0);
        skillsStyle_tog.body.grad_color = LV_COLOR_MAKE(0, 100, 0);

        lv_style_copy(&autonBtnStylePr, &lv_style_plain);
        autonBtnStylePr.body.main_color = LV_COLOR_MAKE(221, 140, 0);
        autonBtnStylePr.body.grad_color = LV_COLOR_MAKE(221, 140, 0);
        autonBtnStylePr.body.radius = 1;
        autonBtnStylePr.text.color = LV_COLOR_WHITE;
        lv_style_copy(&autonBtnStyleRel, &autonBtnStylePr);
        autonBtnStyleRel.body.main_color = LV_COLOR_MAKE(135, 85, 0);
        autonBtnStyleRel.body.grad_color = LV_COLOR_MAKE(135, 85, 0);
        autonBtnStyleRel.text.color = LV_COLOR_WHITE;
        lv_style_copy(&noAutonBtnStyle, &autonBtnStyleRel);
        noAutonBtnStyle.text.color = LV_COLOR_MAKE(130, 130, 130);
        
        lv_style_copy(&lineStyle_Blue, &lv_style_plain);
        lineStyle_Blue.line.color = LV_COLOR_MAKE(0, 0, 255);
        lv_style_copy(&lineStyle_Red, &lv_style_plain);
        lineStyle_Red.line.color = LV_COLOR_MAKE(255, 0, 0);
        lv_style_copy(&lineStyle_Skills, &lv_style_plain);
        lineStyle_Skills.line.color = LV_COLOR_MAKE(0, 255, 0);
    ////////////////////////////

    // Settings Styles //
        lv_style_copy(&sliderKnobStyle, &lv_style_plain);
        sliderKnobStyle.body.border.width = 5;
        sliderKnobStyle.body.border.color = LV_COLOR_MAKE(100, 100, 100);
        sliderKnobStyle.body.main_color = LV_COLOR_MAKE(72, 139, 247);
        sliderKnobStyle.body.grad_color = LV_COLOR_MAKE(72, 139, 247);
        sliderKnobStyle.body.radius = LV_RADIUS_CIRCLE;
        lv_style_copy(&sliderIndicStyle, &lv_style_plain);
        sliderIndicStyle.body.border.width = 7;
        sliderIndicStyle.body.border.color = LV_COLOR_MAKE(57, 250, 74);
        lv_style_copy(&razeCustomStyle, &lv_style_plain);
        razeCustomStyle.body.main_color = LV_COLOR_MAKE(254, 192, 106);
        razeCustomStyle.body.grad_color = LV_COLOR_MAKE(173, 106, 75);
        razeCustomStyle.body.radius = 1;
        razeCustomStyle.text.color = LV_COLOR_WHITE;
        lv_style_copy(&razeCustomStylePre, &razeCustomStyle);
        razeCustomStylePre.body.main_color = LV_COLOR_MAKE(254, 192, 106);
        razeCustomStylePre.body.grad_color = LV_COLOR_MAKE(173, 106, 75);
        razeCustomStylePre.body.radius = 10;
        lv_style_copy(&dropdownStyle, &lv_style_plain);
        dropdownStyle.body.main_color = LV_COLOR_BLACK;
        dropdownStyle.body.grad_color = LV_COLOR_BLACK;
        dropdownStyle.body.border.width = 3;
        dropdownStyle.body.border.color = LV_COLOR_MAKE(0, 112, 192);
        dropdownStyle.text.color = LV_COLOR_WHITE;
        dropdownStyle.body.padding.ver = 10;
    ////////////////////////////

    // Motor Styles for Device Check //
        lv_style_copy(&offline, &lv_style_plain);
        offline.body.border.color = LV_COLOR_RED;
        offline.body.main_color = LV_COLOR_RED;
        offline.body.grad_color = LV_COLOR_RED;
        offline.body.radius = 1;
        offline.text.color = LV_COLOR_WHITE;
        lv_style_copy(&hot, &offline);
        hot.body.border.color = LV_COLOR_ORANGE;
        hot.body.main_color = LV_COLOR_ORANGE;
        hot.body.grad_color = LV_COLOR_ORANGE;
        lv_style_copy(&online, &offline);
        online.body.border.color = LV_COLOR_GREEN;
        online.body.main_color = LV_COLOR_GREEN;
        online.body.grad_color = LV_COLOR_GREEN;
        lv_style_copy(&unknown, &offline);
        unknown.body.border.color = LV_COLOR_YELLOW;
        unknown.body.main_color = LV_COLOR_YELLOW;
        unknown.body.grad_color = LV_COLOR_YELLOW;
    ////////////////////////////

    // Manual Functions Styles //
        lv_style_copy(&manualBtnRel, &lv_style_plain);
        manualBtnRel.body.main_color = LV_COLOR_MAKE(238, 238, 238);
        manualBtnRel.body.grad_color = LV_COLOR_MAKE(238, 238, 238);
        manualBtnRel.body.border.color = LV_COLOR_MAKE(24, 84, 182);
        manualBtnRel.body.border.width = 2;
        manualBtnRel.text.color = LV_COLOR_BLACK;
        manualBtnRel.body.radius = 2;
        lv_style_copy(&manualBtnPre, &lv_style_plain);
        manualBtnPre.body.main_color = LV_COLOR_MAKE(200, 200, 200);
        manualBtnPre.body.grad_color = LV_COLOR_MAKE(200, 200, 200);
        manualBtnPre.body.border.color = LV_COLOR_MAKE(24, 84, 182);
        manualBtnPre.body.border.width = 2;
        manualBtnPre.body.radius = 8;
        lv_style_copy(&manualBtnNoFunc, &lv_style_plain);
        manualBtnNoFunc.body.main_color = LV_COLOR_MAKE(150, 150, 150);
        manualBtnNoFunc.body.grad_color = LV_COLOR_MAKE(150, 150, 150);
        manualBtnNoFunc.body.border.color = LV_COLOR_BLACK;
        manualBtnNoFunc.body.radius = 0;
    ////////////////////////////

    // Init for Device Check Labels //
    std::string port, reversed, gearset, expander;
    for(int j=0; j<3; j++)
    {
        for (int i=0; i<max(max(NUMBER_OF_MOTORS, NUMBER_OF_SENSORS), max(NUMBER_OF_SINGLE3WIRE, NUMBER_OF_DUAL3WIRE)); i++) 
        {
            // The data of the devices will come from the devices class in device.cpp
            if(deviceStrings[j][i].name == "") {break;}
            if (j==TYPE_MOTOR) {
                port = std::to_string(rand() % 20 + 1);
                reversed = ((rand() % 2) == 0) ? "T" : "F";
                gearset = ((rand() % 2) == 0) ? "06" : "36";
                deviceStrings[j][i].stat.append(port + reversed + gearset);
            } else if (j==TYPE_SENSOR) {
                port = std::to_string(rand() % 20 + 1);
                deviceStrings[j][i].stat.append(port);
            } else if (j==TYPE_ADI) {
                expander = ((rand() % 2) == 0) ? "T" : "F";
                port = std::to_string(rand() % 8 + 1);
                deviceStrings[j][i].stat.append(expander + port);
            }
        }
    }

        // short int breaksLocation[3][8-1] {
        //     {0,0,0,0,0,0,0},    // Motors
        //     {0,0,0,0,0,0,-9},   // V% Devices
        //     {0,0,0,0,0,0,-9}    // 3-Wire Devices
        // };
        // short int breaks = 0;
        // short int lastBreak = 0;
        // char dInfoTemp_c[13];
        // string dInfoTemp_cpp;

        // for(int i=0;i<3;i++)
        // {
        //     short int j = 0;
        //     do {
        //         breaksLocation[i][j] = labelStrings[i+6].find("\n", lastBreak);
        //         lastBreak = labelStrings[i+6].find("\n", lastBreak) + 2;
        //         j++; breaks++;
        //     } while (labelStrings[i+6].find("\n", lastBreak) != -1);
            
        //     lastBreak=0;
            
        //     changingLabel[i+7] = "";
        //     for(int k=0;k<=breaks;k++)
        //     {
        //         changingLabel[i+7] += labelStrings[i+6].substr(lastBreak, breaksLocation[i][k]-lastBreak);
        //         lastBreak = breaksLocation[i][k]+1;
        //         if(i==0) // Motors
        //         {
        //             // std::sprintf(dInfoTemp_c, "%i - %i - %i", PORT5, REVERSE, GEARING);
        //             std::sprintf(dInfoTemp_c, "P# - R? - GR");
        //             dInfoTemp_cpp = dInfoTemp_c;
        //             changingLabel[i+7].append("; " + dInfoTemp_cpp);
        //         }
        //         else if(i==1) // Devices
        //         {
        //             if(/*Rotation_Sensor*/0)
        //             {
        //                 // sprintf(dInfoTemp_c, "%i, %i", PORT5, REVERSE);
        //                 sprintf(dInfoTemp_c, "P#, R?");
        //                 changingLabel[i+7].append(dInfoTemp_c);
        //             }
        //             else
        //             {
        //                 // sprintf(dInfoTemp_c, "%i", PORT5);
        //                 sprintf(dInfoTemp_c, "P#");
        //                 dInfoTemp_cpp = dInfoTemp_c;
        //                 changingLabel[i+7].append("; " + dInfoTemp_cpp);   
        //             }
        //         }
        //         else // 3-Wire
        //         {
        //             // sprintf(dInfoTemp_c, "(exp-%i) %c", EXPPORT, PORT3);
        //             sprintf(dInfoTemp_c, "(exp-P#) P#");
        //             dInfoTemp_cpp = dInfoTemp_c;
        //             changingLabel[i+7].append("; " + dInfoTemp_cpp);
        //         }
        //         changingLabel[i+7].append("\n");
        //     }
            
        //     breaks = lastBreak = 0;
        // }
    ////////////////////////////

    // Port Changer Labels //
        lv_style_copy(&gear06Style, &lv_style_plain);
        gear06Style.text.color = LV_COLOR_MAKE(0, 0, 255);
        lv_style_copy(&gear18Style, &lv_style_plain);
        gear18Style.text.color = LV_COLOR_MAKE(0, 255, 0);
        lv_style_copy(&gear32Style, &lv_style_plain);
        gear32Style.text.color = LV_COLOR_MAKE(255, 0, 0);
    ////////////////////////////
    
    // Flywheel speed data //
        FILE *flywheel_read = fopen(FLYWHEEL_FILE, "rb");
        if(flywheel_read != NULL && fileExists(FLYWHEEL_FILE)) // If successful, overwrite sliderPos Value
        {
            fread(&sliderPos_fly, sizeof(sliderPos_fly), 2, flywheel_read);
            fclose(flywheel_read);
            printf("GUI:Flywheel -> Overwrote(%2.2f)\n", sliderPos_fly);                                            // <- To write to log file
        }
        else // Cannot read
        {
            fclose(flywheel_read);
            FILE *flywheel_write = fopen(FLYWHEEL_FILE, "wb");
            if(flywheel_write != NULL) // If can write, write defaults
            {
                printf("GUI:Flywheel -> Defaults Writen to SD\n");                                              // <- To write to log file
                fwrite(&sliderPos_fly, sizeof(sliderPos_fly), 1, flywheel_write);   
                fclose(flywheel_write); 
            }
            else {printf("GUI:Flywheel -> NO SD; Loaded Defaults\n");} // Can't Write, Assumes default value        // <- To write to log file
        }
    ////////////////////////////

    // Intake speed data //
        FILE *intake_read = fopen(INTAKE_FILE, "rb");
        if(intake_read != NULL && fileExists(INTAKE_FILE)) // If successful, overwrite sliderPos Value
        {
            fread(&sliderPos_intake, sizeof(sliderPos_intake), 2, intake_read);
            fclose(intake_read);
            printf("GUI:Intake -> Overwrote(%2.2f)\n", sliderPos_intake);                                            // <- To write to log file
        }
        else // Cannot read
        {
            fclose(intake_read);
            FILE *intake_write = fopen(INTAKE_FILE, "wb");
            if(intake_write != NULL) // If can write, write defaults
            {
                printf("GUI:Intake -> Defaults Writen to SD\n");                                              // <- To write to log file
                fwrite(&sliderPos_intake, sizeof(sliderPos_intake), 1, intake_write);   
                fclose(intake_write); 
            }
            else {printf("GUI:Intake -> NO SD; Loaded Defaults\n");} // Can't Write, Assumes default value        // <- To write to log file
        }
    ////////////////////////////

    // Driver Profile Data //
        FILE *profile_read = fopen(DRIVER_PROFILE_FILE, "rb");
        if(profile_read != NULL && fileExists(DRIVER_PROFILE_FILE)) // If successful, overwrite sliderPos Value
        {
            fread(&driverProfile, sizeof(driverProfile), 1, profile_read);
            fclose(profile_read);
            printf("GUI:Driver Profile -> Overwrote(%d)\n", driverProfile);                                            // <- To write to log file
        }
        else // Cannot read
        {
            fclose(profile_read);
            FILE *profile_write = fopen(DRIVER_PROFILE_FILE, "wb");
            if(profile_write != NULL) // If can write, write defaults
            {
                printf("GUI:Driver Profile -> Defaults Writen to SD\n");                                              // <- To write to log file
                fwrite(&driverProfile, sizeof(driverProfile), 1, profile_write);   
                fclose(profile_write); 
            }
            else {printf("GUI:Driver Profile -> NO SD; Loaded Defaults\n");} // Can't Write, Assumes default value        // <- To write to log file
        }
    ////////////////////////////

    // Initialize common objects //
        robotIcon_init();
        backround = createBtn(lv_scr_act(), 0, 0, SCREEN_WIDTH_MAX, SCREEN_HEIGHT_MAX,
            &plain_black, &plain_black, LV_BTN_ACTION_LONG_PR_REPEAT, NULL, 0, "");

        illiniCR = createBtn(lv_scr_act(), 0, SCREEN_HEIGHT_MAX-25, 80, 30, &topInfo2, &topInfo2,
            NULL, NULL, 0, "ILLINI");
    ////////////////////////////

    guiControlLoop();
}

/// Runs the screen saver in its own task to prevent lockup of program ///
void brain_gui::raze_ss_runner()
{
    while(runRaze)
    {
        lastClick = pros::c::screen_touch_status();
        if(delayCountOne >= razeBlickDelay)
        {
            if(rand()%2 == 0)
                lv_img_set_src(razeImg, &LeftOpen);
            else
                lv_img_set_src(razeImg, &RightOpen);
            pros::delay(100);
            lv_img_set_src(razeImg, &BothClosed);
            pros::delay(100);
            lv_img_set_src(razeImg, &BothOpen);
            razeBlickDelay = ((double)((rand()%4) + 1.5) + (double)((rand()%8)/10.0));  // Blinking (1.5-5.5)
            delayCountOne = 0.0;
        }
        else
        {
            if(glitch)
            {
                delayCountOne += 0.033;
                lv_obj_align(razeImg, NULL, LV_ALIGN_CENTER, (rand() % 21 + (-10)), 0);
                pros::Task::delay(33);
            }
            else
            {
                delayCountOne += 0.05;
                pros::Task::delay(50);
            }
        }

        if(lastClick.touch_status == pros::E_TOUCH_HELD)
            runRaze = false;      
    }

    currentScreen = MAINMENU;
    updateGUI(sdcard); // <-- Random Button to pass to method
}

void brain_gui::guiControlLoop()
{
    // currentScreen = MAINMENU;
    // int waiter = 0;
    // guiLastUpdate = chrono::high_resolution_clock::now();

    // runMainGUI();
    // while(1)
    // {
    //     guiCurrentTime = chrono::high_resolution_clock::now();
    //     auto TimeElapsed = chrono::duration_cast<chrono::milliseconds>(guiCurrentTime-guiLastUpdate);
    //     update_TimeEslaped = (int)TimeElapsed.count();

    //     if(currentScreen == MAINMENU || currentScreen == SETTINGS || currentScreen == DEVICECHECK || currentScreen == ODOMMAP)
    //     {
    //         if(waiter > 2000)
    //         {
    //             lastScreen_del();
    //             waiter = 0;
    //             runMainGUI();
    //         }
    //     }
    //     else if(updated) // If the screen was updated independently of this loop
    //     {
    //         waiter = 0;
    //         updated = false;
    //     }

    //     if(update_TimeEslaped > 5000)
    //     {
    //         currentScreen = RAZE_SS;
    //         glitch = false;
    //         lastScreen = currentScreen;
    //         printf("Starting ScreenSaver\n");
    //         // lastScreen_del(lastScreen);

    //         runMainGUI();
    //     }

    //     waiter += 15;
    //     pros::Task::delay(15);
    // }
}

/**
* Main GUI Method
*/
void brain_gui::runMainGUI()
{
    updateLabel();
    updated = true;
    switch(currentScreen)
    {
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//        
        case(LOGOSCREEN):
            // illiniLogo = lv_img_create(lv_scr_act(), NULL);
            // lv_img_set_src(illiniLogo, &OliveMinestron);
            // lv_obj_align(illiniLogo, NULL, LV_ALIGN_CENTER, 0, 0);

            // lv_obj_align(illiniCR, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
            // returnMainMenuBtn = createBtn(lv_scr_act(), 0, 0, 125, 25, 
            //     &brainBTNpressed_bl, &brainBTNrelease_bl, LV_BTN_ACTION_CLICK, updateGUI, CO_BTN_MAINMENU, "Main Menu");
            // lv_obj_align(returnMainMenuBtn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(MAINMENU):
            brain_gui::isMotor = false;
            brain_gui::isRotation = false;
            brain_gui::isV5Port = false;
            brain_gui::isDeviceSelected = false;

            illiniLogoBtn = lv_imgbtn_create(lv_scr_act(), NULL);
            lv_imgbtn_set_src(illiniLogoBtn, LV_BTN_STATE_REL, &simpleIlliniLogo);
            lv_imgbtn_set_src(illiniLogoBtn, LV_BTN_STATE_PR, &simpleIlliniLogo);
            lv_btn_set_action(illiniLogoBtn, LV_BTN_ACTION_CLICK, updateGUI);
            lv_obj_align(illiniLogoBtn, NULL, LV_ALIGN_CENTER, 0, -43);
            lv_obj_set_free_num(illiniLogoBtn, MM_BTN_ILLINILOGO);
            setAutonBtn = createBtn(lv_scr_act(), 0, 160-10, 135, 40, &brainBTNpressed_og, &brainBTNrelease_og, 
                LV_BTN_ACTION_CLICK, updateGUI, MM_BTN_SETAUTON, "Set Auton");
            settingBtn = createBtn(lv_scr_act(), 173, 160-10, 135, 40, &brainBTNpressed_bl, &brainBTNrelease_bl,
                LV_BTN_ACTION_CLICK, updateGUI, MM_BTN_SETTING, "Settings");
            deviceCheckBtn = createBtn(lv_scr_act(), 345, 160-10, 135, 40, &brainBTNpressed_og, &brainBTNrelease_og,
                LV_BTN_ACTION_CLICK,updateGUI, MM_BTN_DEVICECHECK, "Device Check");
            PortChangerBtn = createBtn(lv_scr_act(), 173, SCREEN_HEIGHT_MAX-40, 135, 40, &brainBTNpressed_og, &brainBTNrelease_og,
                LV_BTN_ACTION_CLICK, updateGUI, MM_BTN_PORTCHANGER, "Port Changer");
            manualCtrlBtn = createBtn(lv_scr_act(), 345, SCREEN_HEIGHT_MAX-40, 135, 40, &brainBTNpressed_bl, &brainBTNrelease_bl,
                LV_BTN_ACTION_CLICK, updateGUI, MM_BTN_MANUALCTRL, "Manual Ctrl");
            odomMapBtn = createBtn(lv_scr_act(), 0, SCREEN_HEIGHT_MAX-40, 135, 40, &brainBTNpressed_bl, &brainBTNrelease_bl,
                NULL, NULL, MM_BTN_ODOMMAP, "Odom Map");
            batteryInfo = createBtn(lv_scr_act(), 83, 0, 105, 30, &topInfo, &topInfo,
                NULL, NULL, MM_BTNLABEL_BATTERYINFO, changingLabel[MM_BTNLABEL_BATTERYINFO-10].c_str());
            sdcard = createBtn(lv_scr_act(), 188, 0, 105, 30, &topInfo, &topInfo,
                NULL, NULL, MM_BTNLABEL_SDCARD, changingLabel[MM_BTNLABEL_SDCARD-10].c_str());
            robotType = createBtn(lv_scr_act(), 293, 0, 105, 30, &topInfo, &topInfo,
                NULL, NULL, MM_BTNLABEL_ROBOTTYPE, changingLabel[MM_BTNLABEL_ROBOTTYPE-2].c_str());

            if(autonColor == 0 && autonType == 0)
            {
                errorBox = createBtn(lv_scr_act(), 108, 122, 265, 20, &errorBoxStyle, &errorBoxStyle, NULL, NULL, 999, "NO AUTON SELECTED");
                // lv_obj_align(errorBox, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -110);
            }
            else if(deviceConnectionError)
            {
                errorBox = createBtn(lv_scr_act(), 108, 122, 265, 20, &errorBoxStyle, &errorBoxStyle, NULL, NULL, 999, "CHECK CONNECTIONS");
                // lv_obj_align(errorBox, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -110);
            }
            else if(autonColor == 0 && autonType == 0 && !deviceConnectionError)
            {
                errorBox = createBtn(lv_scr_act(), 108, 122, 265, 20, &errorBoxStyle, &errorBoxStyle, NULL, NULL, 999, "NO AUTON / CONNECTION WARN");
                // lv_obj_align(errorBox, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
            }
            else
            {
                errorBox = lv_btn_create(lv_scr_act(), NULL);
                errorBox->hidden = true;
                lv_obj_align(illiniLogoBtn, NULL, LV_ALIGN_CENTER, 0, -28);
            }
            illiniCR->hidden = true;
            // errorBox->hidden = true;
            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(TEAMINFO):
            illiniLogo = lv_img_create(lv_scr_act(), NULL);
            lv_img_set_src(illiniLogo, &IlliniLogoMain);
            lv_obj_align(illiniLogo, NULL, LV_ALIGN_CENTER, 0, 0);

            illiniCR->hidden = true;
            returnMainMenuBtn = createBtn(lv_scr_act(), 0, 0, 125, 25, 
                &brainBTNpressed_bl, &brainBTNrelease_bl, LV_BTN_ACTION_CLICK, updateGUI, CO_BTN_MAINMENU, "Main Menu");
            lv_obj_align(returnMainMenuBtn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(SETAUTON):
            chooseLable = createLabel(lv_scr_act(), 0, 0, 150, 30, &topInfo2, "Choose Auton;");
            lv_obj_align(chooseLable, NULL, LV_ALIGN_IN_TOP_LEFT, (SCREEN_WIDTH_MAX/2)-((SCREEN_WIDTH_MAX/2)/2)-50, 15);

            matchAuton = createBtn(lv_scr_act(), 10, 50, 80+10+80, 30, &matchAutonStyle_tog, &matchAutonStyle, 
                LV_BTN_ACTION_CLICK, updateGUI, SA_BTN_MATCHAUTON, "Red / Blue");
            lv_btn_set_toggle(matchAuton, true);
            if(autonColor == 1)
                lv_btn_set_state(matchAuton, LV_BTN_STYLE_REL);
            else
                lv_btn_set_state(matchAuton, LV_BTN_STATE_PR);

            skills = createBtn(lv_scr_act(), 10+80+10+80+10, 50, 80, 30, &skillsStyle_tog, &skillsStyle,
                LV_BTN_ACTION_CLICK, updateGUI, SA_BTN_SKILLS, "SKILLS");
            lv_btn_set_toggle(skills, true);
            if(autonColor == 3)
                lv_btn_set_state(skills, LV_BTN_STYLE_REL);
            else
                lv_btn_set_state(skills, LV_BTN_STATE_PR);

            autonOne = createBtn(lv_scr_act(), 30, (SCREEN_HEIGHT_MAX/2)-20, 100, 30, &autonBtnStylePr, &autonBtnStyleRel,
                LV_BTN_ACTION_CLICK, updateGUI, SA_BTN_AUTONONE, labelStrings[0].c_str());
            lv_btn_set_toggle(autonOne, true);
            if(autonType == 1)
                lv_btn_set_state(autonOne, LV_BTN_STATE_PR);
            else
                lv_btn_set_state(autonOne, LV_BTN_STATE_REL);

            autonTwo = createBtn(lv_scr_act(), 30, (SCREEN_HEIGHT_MAX/2)+20, 100, 30, &noAutonBtnStyle, &noAutonBtnStyle,
                LV_BTN_ACTION_CLICK, NULL, SA_BTN_AUTONTWO, labelStrings[1].c_str());
            // lv_btn_set_toggle(autonTwo, true);
            // if(autonType == 2)
            //     lv_btn_set_state(autonTwo, LV_BTN_STATE_PR);
            // else
            //     lv_btn_set_state(autonTwo, LV_BTN_STATE_REL);

            autonThree = createBtn(lv_scr_act(), 30, (SCREEN_HEIGHT_MAX/2)+60, 100, 30, &noAutonBtnStyle, &noAutonBtnStyle,
                LV_BTN_ACTION_CLICK, NULL, SA_BTN_AUTONTHREE, labelStrings[2].c_str());
            // lv_btn_set_toggle(autonThree, true);
            // if(autonType == 3)
            //     lv_btn_set_state(autonThree, LV_BTN_STATE_PR);
            // else
            //     lv_btn_set_state(autonThree, LV_BTN_STATE_REL);

            autonFour = createBtn(lv_scr_act(), 150, (SCREEN_HEIGHT_MAX/2)-20, 100, 30, &noAutonBtnStyle, &noAutonBtnStyle,
                LV_BTN_ACTION_CLICK, NULL, SA_BTN_AUTONFOUR, labelStrings[3].c_str());
            // lv_btn_set_toggle(autonFour, true);
            // if(autonType == 4)
                // lv_btn_set_state(autonFour, LV_BTN_STATE_PR);
            // else
            //     lv_btn_set_state(autonFour, LV_BTN_STATE_REL);

            autonFive = createBtn(lv_scr_act(), 150, (SCREEN_HEIGHT_MAX/2)+20, 100, 30, &noAutonBtnStyle, &noAutonBtnStyle,
                LV_BTN_ACTION_CLICK, NULL, SA_BTN_AUTONFIVE, labelStrings[4].c_str());
            // lv_btn_set_toggle(autonFive, true);
            // if(autonType == 5)
                // lv_btn_set_state(autonFive, LV_BTN_STATE_PR);
            // else
            //     lv_btn_set_state(autonFive, LV_BTN_STATE_REL);

            autonSix = createBtn(lv_scr_act(), 150, (SCREEN_HEIGHT_MAX/2)+60, 100, 30, &noAutonBtnStyle, &noAutonBtnStyle,
                LV_BTN_ACTION_CLICK, NULL, SA_BTN_AUTONSIX, labelStrings[5].c_str());
            // lv_btn_set_toggle(autonSix, true);
            // if(autonType == 6)
                // lv_btn_set_state(autonSix, LV_BTN_STATE_PR);
            // else
            //     lv_btn_set_state(autonSix, LV_BTN_STATE_REL);

            fieldMap = lv_img_create(lv_scr_act(), NULL);
            lv_img_set_src(fieldMap, &FieldMapSmall);
            lv_obj_align(fieldMap, NULL, LV_ALIGN_IN_RIGHT_MID, 0, -10); // 285 23
            
            // redside_line = lv_line_create(lv_scr_act(), NULL);
            // blueside_line = lv_line_create(lv_scr_act(), NULL);
            skills_line = lv_line_create(lv_scr_act(), NULL);
            // lv_obj_set_style(redside_line, &lineStyle_Red);
            // lv_obj_set_style(blueside_line, &lineStyle_Blue);
            lv_obj_set_style(skills_line, &lineStyle_Skills);

            //////////////////////////////////////////////////////////////////////////////////
            /// Visit https://pixspy.com/ to draw auton lines. Use "img.png" for brain map ///
            //////////////////////////////////////////////////////////////////////////////////

            if(autonType == 1)
            {
                // if(autonColor == 1) // Match Auton
                // {
                //     autonLinePoints[AUTON_RED_POINTS][0] = {285+1, 13+33};
                //     autonLinePoints[AUTON_RED_POINTS][1] = {285+33, 13+33};
                //     autonLinePoints[AUTON_RED_POINTS][2] = {285+33, 13+65};
                //     autonLinePoints[AUTON_RED_POINTS][3] = {285+1, 13+65};
                //     autonLinePoints[AUTON_RED_POINTS][4] = {285+1, 13+33};
                //     lv_line_set_points(redside_line, autonLinePoints[AUTON_RED_POINTS], 5);

                //     autonLinePoints[AUTON_BLUE_POINTS][0] = {285+161, 13+129};
                //     autonLinePoints[AUTON_BLUE_POINTS][1] = {285+193, 13+129};
                //     autonLinePoints[AUTON_BLUE_POINTS][2] = {285+193, 13+161};
                //     autonLinePoints[AUTON_BLUE_POINTS][3] = {285+161, 13+161};
                //     autonLinePoints[AUTON_BLUE_POINTS][4] = {285+161, 13+129};
                //     lv_line_set_points(blueside_line, autonLinePoints[AUTON_BLUE_POINTS], 5);
                // }
                // else
                // {
                //     lv_obj_del(redside_line);
                //     lv_obj_del(blueside_line);
                // }
            }
            else if(autonType == 2)
            {
                // if(autonColor == 1) // Match Auton
                // {
                //     autonLinePoints[AUTON_RED_POINTS][0] = {285+96, 13+161};
                //     autonLinePoints[AUTON_RED_POINTS][1] = {285+161, 13+161};
                //     autonLinePoints[AUTON_RED_POINTS][2] = {285+161, 13+193};
                //     autonLinePoints[AUTON_RED_POINTS][3] = {285+96, 13+193};
                //     autonLinePoints[AUTON_RED_POINTS][4] = {285+96, 13+161};
                //     lv_line_set_points(redside_line, autonLinePoints[AUTON_RED_POINTS], 5);

                //     autonLinePoints[AUTON_BLUE_POINTS][0] = {285+33, 13+33};
                //     autonLinePoints[AUTON_BLUE_POINTS][1] = {285+98, 13+33};
                //     autonLinePoints[AUTON_BLUE_POINTS][2] = {285+98, 13+1};
                //     autonLinePoints[AUTON_BLUE_POINTS][3] = {285+33, 13+1};
                //     autonLinePoints[AUTON_BLUE_POINTS][4] = {285+33, 13+33};
                //     lv_line_set_points(blueside_line, autonLinePoints[AUTON_BLUE_POINTS], 5);
                // }
                // else
                // {
                //     lv_obj_del(redside_line);
                //     lv_obj_del(blueside_line);
                // }
            }
            else if(autonType == 3)
            {
                // if(autonColor == 1) // Match Auton
                // {
                //     autonLinePoints[AUTON_RED_POINTS][0] = {285+1, 13+33};
                //     autonLinePoints[AUTON_RED_POINTS][1] = {285+33, 13+33};
                //     autonLinePoints[AUTON_RED_POINTS][2] = {285+161, 13+161};
                //     autonLinePoints[AUTON_RED_POINTS][3] = {285+161, 13+193};
                //     autonLinePoints[AUTON_RED_POINTS][4] = {285+129, 13+193};
                //     autonLinePoints[AUTON_RED_POINTS][5] = {285+129, 13+177};
                //     autonLinePoints[AUTON_RED_POINTS][6] = {285+18, 13+65};
                //     autonLinePoints[AUTON_RED_POINTS][7] = {285+1, 13+65};
                //     autonLinePoints[AUTON_RED_POINTS][8] = {285+1, 13+33};
                //     lv_line_set_points(redside_line, autonLinePoints[AUTON_RED_POINTS], 9);

                //     autonLinePoints[AUTON_BLUE_POINTS][0] = {285+193, 13+161};
                //     autonLinePoints[AUTON_BLUE_POINTS][1] = {285+161, 13+161};
                //     autonLinePoints[AUTON_BLUE_POINTS][2] = {285+33, 13+33};
                //     autonLinePoints[AUTON_BLUE_POINTS][3] = {285+33, 13+1};
                //     autonLinePoints[AUTON_BLUE_POINTS][4] = {285+65, 13+1};
                //     autonLinePoints[AUTON_BLUE_POINTS][5] = {285+65, 13+18};
                //     autonLinePoints[AUTON_BLUE_POINTS][6] = {285+177, 13+129};
                //     autonLinePoints[AUTON_BLUE_POINTS][7] = {285+193, 13+129};
                //     autonLinePoints[AUTON_BLUE_POINTS][8] = {285+193, 13+161};
                //     lv_line_set_points(blueside_line, autonLinePoints[AUTON_BLUE_POINTS], 9);
                // }
                // else
                // {
                //     lv_obj_del(redside_line);
                //     lv_obj_del(blueside_line);
                // }
            }
            else if(autonType == 4)
            {
                // if(autonColor == 1) // Match Auton
                // {
                //     autonLinePoints[AUTON_RED_POINTS][0] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][1] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][2] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][3] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][4] = {0, 0};
                //     lv_line_set_points(redside_line, autonLinePoints[AUTON_RED_POINTS], 9);

                //     autonLinePoints[AUTON_BLUE_POINTS][0] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][1] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][2] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][3] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][4] = {0, 0};
                //     lv_line_set_points(blueside_line, autonLinePoints[AUTON_BLUE_POINTS], 9);
                // }
                // else
                // {
                //     lv_obj_del(redside_line);
                //     lv_obj_del(blueside_line);
                // }
            }
            else if(autonType == 5)
            {
                // if(autonColor == 1) // Match Auton
                // {
                //     autonLinePoints[AUTON_RED_POINTS][0] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][1] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][2] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][3] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][4] = {0, 0};
                //     lv_line_set_points(redside_line, autonLinePoints[AUTON_RED_POINTS], 9);

                //     autonLinePoints[AUTON_BLUE_POINTS][0] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][1] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][2] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][3] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][4] = {0, 0};
                //     lv_line_set_points(blueside_line, autonLinePoints[AUTON_BLUE_POINTS], 9);
                // }
                // else
                // {
                //     lv_obj_del(redside_line);
                //     lv_obj_del(blueside_line);
                // }
            }
            else if(autonType == 6)
            {
                // if(autonColor == 1) // Match Auton
                // {
                //     autonLinePoints[AUTON_RED_POINTS][0] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][1] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][2] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][3] = {0, 0};
                //     autonLinePoints[AUTON_RED_POINTS][4] = {0, 0};
                //     lv_line_set_points(redside_line, autonLinePoints[AUTON_RED_POINTS], 9);

                //     autonLinePoints[AUTON_BLUE_POINTS][0] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][1] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][2] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][3] = {0, 0};
                //     autonLinePoints[AUTON_BLUE_POINTS][4] = {0, 0};
                //     lv_line_set_points(blueside_line, autonLinePoints[AUTON_BLUE_POINTS], 9);
                // }
                // else
                // {
                //     lv_obj_del(redside_line);
                //     lv_obj_del(blueside_line);
                // }
            }
            else if(autonColor == 3) // Skills
            { 
                autonLinePoints[AUTON_SKILLS_POINTS][0] = {285, 45-32};
                autonLinePoints[AUTON_SKILLS_POINTS][1] = {479, 45-32};
                autonLinePoints[AUTON_SKILLS_POINTS][2] = {479, 239-32};
                autonLinePoints[AUTON_SKILLS_POINTS][3] = {285, 239-32};
                autonLinePoints[AUTON_SKILLS_POINTS][4] = {285, 45-32};
                lv_line_set_points(skills_line, autonLinePoints[AUTON_SKILLS_POINTS], 5);
            }

            lv_obj_align(illiniCR, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
            returnMainMenuBtn = createBtn(lv_scr_act(), 0, 0, 125, 25, 
                &brainBTNpressed_bl, &brainBTNrelease_bl, LV_BTN_ACTION_CLICK, updateGUI, CO_BTN_MAINMENU, "Main Menu");
            lv_obj_align(returnMainMenuBtn, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -35, 0);

            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(SETTINGS):
            /**
            * @note Add do functions method to run functions requested
            */
            inertialCaliBtn = createBtn(lv_scr_act(), 15, 15, 140, 30, &brainBTNpressed, &brainBTNrelease,
                LV_BTN_ACTION_CLICK, updateGUI, S_BTN_INERTIALCALI, "Calibrate Gyro");

            manualAutonBtn = createBtn(lv_scr_act(), 15+140+25, 15, 135, 30, &brainBTNpressed, &brainBTNrelease,
                LV_BTN_ACTION_CLICK, updateGUI, S_BTN_MANUALAUTON, "Manual Auton");

            razeBtn = createBtn(lv_scr_act(), 0, 0, 100, 30, &razeCustomStylePre, &razeCustomStyle,
                LV_BTN_ACTION_CLICK, updateGUI, S_BTN_RAZE, "Raze");
            lv_obj_align(razeBtn, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

            gyroInfo = createLabel(lv_scr_act(), 15, 15+30+10, 140, 50, &topInfo2, changingLabel[S_LABEL_GYROINFO-32].c_str());

            driverProfileDD = lv_ddlist_create(lv_scr_act(), NULL);
            lv_obj_set_pos(driverProfileDD, 15+140+25+140+5, 15);
            lv_obj_set_size(driverProfileDD, 190, 50);
            lv_ddlist_set_draw_arrow(driverProfileDD, true);
            lv_ddlist_set_hor_fit(driverProfileDD, true);
            lv_ddlist_set_align(driverProfileDD, LV_LABEL_ALIGN_CENTER);
            lv_ddlist_set_style(driverProfileDD, LV_DDLIST_STYLE_BG, &dropdownStyle);
            lv_ddlist_set_style(driverProfileDD, LV_DDLIST_STYLE_SEL, &dropdownStyle);
            lv_ddlist_set_options(driverProfileDD, "Tank Main  \nArcade Main  \nDriver 1  \nDriver 2  "); // 3 Spaces
            lv_ddlist_set_selected(driverProfileDD, driverProfile);
            lv_ddlist_set_action(driverProfileDD, saveLastDriveProfile);

            if(autonColor != 0)
                if(autonType != 0)
                {
                    manualAutonStateLabel = createLabel(lv_scr_act(), 180, 55, 140, 50, &topInfo2, changingLabel[S_LABEL_MANUALAUTONSTATE-34].c_str());
                    lv_label_set_align(manualAutonStateLabel, LV_LABEL_ALIGN_CENTER);
                    lv_obj_align(manualAutonStateLabel, manualAutonBtn, LV_ALIGN_CENTER, 0, 40);
                }
                else
                    manualAutonStateLabel = createLabel(lv_scr_act(), 180, 55, 140, 50, &topInfo2, "Skills Auton");
            else
            {
                manualAutonStateLabel = createLabel(lv_scr_act(), 180, 55, 140, 50, &topInfo2, "NO AUTON\nSELC");
                lv_label_set_align(manualAutonStateLabel, LV_LABEL_ALIGN_CENTER);
                lv_obj_align(manualAutonStateLabel, manualAutonBtn, LV_ALIGN_CENTER, 0, 40);
            }

            // flywheelSpeedBtn = createBtn(lv_scr_act(), mineStrone->coords.x1, (SCREEN_HEIGHT_MAX/2), 135, 30, &topInfo, &topInfo,
            //     NULL, NULL, 999, "");
            // intakeSpeedBtn = createBtn(lv_scr_act(), manualAutonBtn->coords.x1, (SCREEN_HEIGHT_MAX/2), 135, 30, &topInfo, &topInfo,
            //     NULL, NULL, 999, "");

            // flywheelSpeed = lv_slider_create(lv_scr_act(), NULL);
            // lv_obj_set_pos(flywheelSpeed, 260, 160);
            // lv_slider_set_range(flywheelSpeed, 0, 48);
            // lv_slider_set_action(flywheelSpeed, flySpeedUpdate);
            // lv_slider_set_style(flywheelSpeed, LV_SLIDER_STYLE_KNOB, &sliderKnobStyle);
            // lv_slider_set_style(flywheelSpeed, LV_SLIDER_STYLE_INDIC, &sliderIndicStyle);
            // lv_obj_set_size(flywheelSpeed, (SCREEN_WIDTH_MAX/2)-35, 40);

            // intakeSpeed = lv_slider_create(lv_scr_act(), NULL);
            // lv_obj_set_pos(intakeSpeed, 25, 160);
            // lv_slider_set_range(intakeSpeed, 0, 48);
            // lv_slider_set_action(intakeSpeed, intakeSpeedUpdate);
            // lv_slider_set_style(intakeSpeed, LV_SLIDER_STYLE_KNOB, &sliderKnobStyle);
            // lv_slider_set_style(intakeSpeed, LV_SLIDER_STYLE_INDIC, &sliderIndicStyle);
            // lv_obj_set_size(intakeSpeed, (SCREEN_WIDTH_MAX/2)-35, 40);

            // flywheelSpeedLabel = lv_label_create(lv_scr_act(), NULL);
            // lv_obj_set_pos(flywheelSpeedLabel, flywheelSpeedBtn->coords.x1+10, flywheelSpeedBtn->coords.y1+5);    /*Align top of the slider*/
            // intakeSpeedLabel = lv_label_create(lv_scr_act(), NULL);
            // lv_obj_set_pos(intakeSpeedLabel, intakeSpeedBtn->coords.x1+10, intakeSpeedBtn->coords.y1+5);    /*Align top of the slider*/

            // if(startUpInit)
            // {
            //     flySpeedUpdate(flywheelSpeed);
            //     startUpInit = true;
            //     intakeSpeedUpdate(intakeSpeed);
            // }

            // lv_slider_set_value(flywheelSpeed, sliderPos_fly*4);
            // lv_label_set_text(flywheelSpeedLabel, sliderPos_fly_str);
            // lv_slider_set_value(intakeSpeed, sliderPos_intake*4);
            // lv_label_set_text(intakeSpeedLabel, sliderPos_intake_str);

            lv_obj_align(illiniCR, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
            returnMainMenuBtn = createBtn(lv_scr_act(), 0, 0, 125, 25, 
                &brainBTNpressed_bl, &brainBTNrelease_bl, LV_BTN_ACTION_CLICK, updateGUI, CO_BTN_MAINMENU, "Main Menu");
            lv_obj_align(returnMainMenuBtn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(RAZE_SS):
            frameObj = lv_img_create(lv_scr_act(), NULL);
            lv_img_set_src(frameObj, &Frame);
            lv_obj_align(frameObj, NULL, LV_ALIGN_CENTER, 0, 0);
            razeImg = lv_img_create(lv_scr_act(), NULL);
            lv_img_set_src(razeImg, &BothOpen);
            lv_obj_align(razeImg, NULL, LV_ALIGN_CENTER, 0, 0);
            runRaze = true;

            if(pros::competition::is_connected())
                glitch = true;
            else
                glitch = false;
            // if (pros::competition::get_status() & (COMPETITION_DISABLED == false))
            //     glitch = true;
            // else
            //     glitch = false;

            pros::delay(1000); // Prevent instaneous close of screensaver
            
            {
                pros::Task raze_ss_task (BrainGUI.raze_ss_runner);
                raze_ss_task.set_priority(6);
            }
            break;              
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(DEVICECHECK):
            // motorBtnTitle = createBtn(lv_scr_act(), (SCREEN_WIDTH_MAX/4)-50-35, 5, 100, 30, &topInfo, &topInfo, 
            //     NULL, NULL, DC_BTNLABEL_MOTORTITLE, "Motors");
            // sensorBtnTitle = createBtn(lv_scr_act(), (SCREEN_WIDTH_MAX/2)-50, 5, 100, 30, &topInfo, &topInfo, 
            //     NULL, NULL, DC_BTNLABEL_SENSORTITLE, "Sensors");
            // ADIBtnTitle = createBtn(lv_scr_act(), (SCREEN_WIDTH_MAX*3/4)-50+35, 5, 100, 30, &topInfo, &topInfo, 
            //     NULL, NULL, DC_BTNLABEL_ADITITLE, "ADI Sens");

            nameTemp.append(deviceStrings[TYPE_MOTOR][DC_BTNLABEL_FRTM].name + "\n" + 
                deviceStrings[TYPE_MOTOR][DC_BTNLABEL_FRBM].stat);
            frtm_btn = createBtn(lv_scr_act(), 0, 150, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_FRTM, nameTemp.c_str());
            deviceCheck_motorStyle_update(frtm_btn);
            
            nameTemp.append(deviceStrings[TYPE_MOTOR][DC_BTNLABEL_FRBM].name + "\n" + 
                deviceStrings[TYPE_MOTOR][DC_BTNLABEL_FRBM].stat);
            frbm_btn = createBtn(lv_scr_act(), 45, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_FRBM, nameTemp.c_str());
            deviceCheck_motorStyle_update(frbm_btn);

            nameTemp.append(deviceStrings[TYPE_MOTOR][DC_BTNLABEL_BRM].name + "\n" + 
                deviceStrings[TYPE_MOTOR][DC_BTNLABEL_BRM].stat);
            brm_btn = createBtn(lv_scr_act(), 90, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_BRM, nameTemp.c_str());
            deviceCheck_motorStyle_update(brm_btn);
            
            nameTemp.append(deviceStrings[TYPE_MOTOR][DC_BTNLABEL_FLTM].name + "\n" + 
                deviceStrings[TYPE_MOTOR][DC_BTNLABEL_FLTM].stat);
            fltm_btn = createBtn(lv_scr_act(), 135, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_FLTM, nameTemp.c_str());
            deviceCheck_motorStyle_update(fltm_btn);
            
            nameTemp.append(deviceStrings[TYPE_MOTOR][DC_BTNLABEL_FLBM].name + "\n" + 
                deviceStrings[TYPE_MOTOR][DC_BTNLABEL_FLBM].stat);
            flbm_btn = createBtn(lv_scr_act(), 180, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_FLBM, nameTemp.c_str());
            deviceCheck_motorStyle_update(flbm_btn);

            nameTemp.append(deviceStrings[TYPE_MOTOR][DC_BTNLABEL_BLM].name + "\n" + 
                deviceStrings[TYPE_MOTOR][DC_BTNLABEL_BLM].stat);
            blm_btn = createBtn(lv_scr_act(), 225, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_BLM, nameTemp.c_str());
            deviceCheck_motorStyle_update(blm_btn);

            nameTemp.append(deviceStrings[TYPE_MOTOR][DC_BTNLABEL_CATAR].name + "\n" + 
                deviceStrings[TYPE_MOTOR][DC_BTNLABEL_CATAR].stat);
            cataR_btn = createBtn(lv_scr_act(), 270, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_CATAR, nameTemp.c_str());
            deviceCheck_motorStyle_update(cataR_btn);
            
            nameTemp.append(deviceStrings[TYPE_MOTOR][DC_BTNLABEL_CATAL].name + "\n" + 
                deviceStrings[TYPE_MOTOR][DC_BTNLABEL_CATAL].stat);
            cataL_btn = createBtn(lv_scr_act(), 315, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_CATAL, nameTemp.c_str());
            deviceCheck_motorStyle_update(cataL_btn);

            nameTemp.append(deviceStrings[TYPE_MOTOR][DC_BTNLABEL_INTAKE].name + "\n" + 
                deviceStrings[TYPE_MOTOR][DC_BTNLABEL_INTAKE].stat);
            intake_btn = createBtn(lv_scr_act(), 360, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_INTAKE, nameTemp.c_str());
            deviceCheck_motorStyle_update(intake_btn);

            nameTemp.append(deviceStrings[TYPE_SENSOR][DC_BTNLABEL_IMU].name + "\n" + 
                deviceStrings[TYPE_SENSOR][DC_BTNLABEL_IMU].stat);
            imu_btn = createBtn(lv_scr_act(), 405, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_IMU, nameTemp.c_str());
            deviceCheck_motorStyle_update(imu_btn);

            nameTemp.append(deviceStrings[TYPE_SENSOR][DC_BTNLABEL_DISTANCE].name + "\n" + 
                deviceStrings[TYPE_SENSOR][DC_BTNLABEL_DISTANCE].stat);
            distance_btn = createBtn(lv_scr_act(), 450, 0, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_DISTANCE, nameTemp.c_str());
            deviceCheck_motorStyle_update(distance_btn);

            nameTemp.append(deviceStrings[TYPE_ADI][DC_BTNLABEL_LIMIT].name + "\n" + 
                deviceStrings[TYPE_ADI][DC_BTNLABEL_LIMIT].stat);
            limit_btn = createBtn(lv_scr_act(), 0, 75, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_LIMIT, nameTemp.c_str());
            deviceCheck_motorStyle_update(limit_btn);

            nameTemp.append(deviceStrings[TYPE_ADI][DC_BTNLABEL_XENCODER].name + "\n" + 
                deviceStrings[TYPE_ADI][DC_BTNLABEL_XENCODER].stat);
            x_encoder_btn = createBtn(lv_scr_act(), 45, 75, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_XENCODER, nameTemp.c_str());
            deviceCheck_motorStyle_update(x_encoder_btn);

            nameTemp.append(deviceStrings[TYPE_ADI][DC_BTNLABEL_YENCODER].name + "\n" + 
                deviceStrings[TYPE_ADI][DC_BTNLABEL_YENCODER].stat);
            y_encoder_btn = createBtn(lv_scr_act(), 90, 75, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_YENCODER, nameTemp.c_str());
            deviceCheck_motorStyle_update(y_encoder_btn);

            nameTemp.append(deviceStrings[TYPE_ADI][DC_BTNLABEL_FB_PNEU].name + "\n" + 
                deviceStrings[TYPE_ADI][DC_BTNLABEL_FB_PNEU].stat);
            fb_pneu_btn = createBtn(lv_scr_act(), 135, 75, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_FB_PNEU, nameTemp.c_str());
            deviceCheck_motorStyle_update(fb_pneu_btn);

            nameTemp.append(deviceStrings[TYPE_ADI][DC_BTNLABEL_WINGS_PNEU].name + "\n" + 
                deviceStrings[TYPE_ADI][DC_BTNLABEL_WINGS_PNEU].stat);
            wings_pneu_btn = createBtn(lv_scr_act(), 250, 100, 75, 45, NULL, NULL, NULL, NULL, 
                DC_BTNLABEL_WINGS_PNEU, nameTemp.c_str());
            deviceCheck_motorStyle_update(wings_pneu_btn);

            lv_obj_align(illiniCR, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
            returnMainMenuBtn = createBtn(lv_scr_act(), 0, 0, 125, 25, 
                &brainBTNpressed_bl, &brainBTNrelease_bl, LV_BTN_ACTION_CLICK, updateGUI, CO_BTN_MAINMENU, "Main Menu");
            lv_obj_align(returnMainMenuBtn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(PORTCHANGE):
            /**
            * @note Must call for modification of devices array and disable robot after the changes are confirmed
            */
            deviceRol = lv_roller_create(lv_scr_act(), NULL);
            lv_roller_set_align(deviceRol, LV_LABEL_ALIGN_CENTER);
            lv_roller_set_options(deviceRol, (labelStrings[6] + "\n" + labelStrings[7] + "\n" + labelStrings[8]).c_str());
            lv_roller_set_selected(deviceRol, deviceSelected, false);
            lv_roller_set_visible_row_count(deviceRol, 5);
            lv_obj_set_pos(deviceRol, 30, 20);
            lv_roller_set_action(deviceRol, portScreen_roller_update);
            lv_obj_set_free_num(deviceRol, PC_ROLLER_DEVICEROL);

            if(!isDeviceSelected)
            {
                V5_portRol = lv_roller_create(lv_scr_act(), NULL);
                V5_portRol->hidden = true;
            }
            else if(isDeviceSelected && isV5Port)
            {
                V5_portRol = lv_roller_create(lv_scr_act(), NULL);
                lv_roller_set_align(V5_portRol, LV_LABEL_ALIGN_CENTER);
                lv_roller_set_options(V5_portRol, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21");
                lv_roller_set_visible_row_count(V5_portRol, 5);
                lv_obj_set_pos(V5_portRol, 25+deviceRol->coords.x2, deviceRol->coords.y1);
                lv_roller_set_action(V5_portRol, portScreen_roller_update);
            }
            else if(isDeviceSelected && !isV5Port)
            {
                V5_portRol = lv_roller_create(lv_scr_act(), NULL);
                lv_roller_set_align(V5_portRol, LV_LABEL_ALIGN_CENTER);
                lv_roller_set_options(V5_portRol, "A\nB\nC\nD\nE\nF\nG\nH\nEA\nEB\nEC\nED\nEE\nEF\nEG\nEH");
                lv_roller_set_visible_row_count(V5_portRol, 5);
                lv_obj_set_pos(V5_portRol, 25+deviceRol->coords.x2, deviceRol->coords.y1);
                lv_roller_set_action(V5_portRol, portScreen_roller_update);
            }

            deviceCustomTitle = createBtn(lv_scr_act(), 265, 6, 480-285+20, 30, &topInfo, &topInfo, NULL, NULL, 999, "Device Customization");

            if(!isDeviceSelected)
                confirmChangeBtn = createBtn(lv_scr_act(), 280, 145, 90, 75, &brainBTNpressed, &brainBTNrelease,
                    LV_BTN_ACTION_CLICK, updateGUI, PC_BTN_CONFIRMCHANGEBTN, "Edit\nDevice");
            else
                confirmChangeBtn = createBtn(lv_scr_act(), 280, 145, 90, 75, &brainBTNpressed, &brainBTNrelease,
                    LV_BTN_ACTION_LONG_PR_REPEAT, updateGUI, PC_BTN_CONFIRMCHANGEBTN, "Confirm\nChanges\n(Hold)");

            resetDefBtn = createBtn(lv_scr_act(), 10+confirmChangeBtn->coords.x2, 145, 90, 75, &brainBTNpressed, &brainBTNrelease,
                LV_BTN_ACTION_LONG_PR_REPEAT, updateGUI, PC_BTN_RESETDEFBTN, "Reset\nDefaults\n(Hold)");

            if(isMotor)
            {
                reverseSw = lv_sw_create(lv_scr_act(), NULL);
                lv_obj_set_pos(reverseSw, 295, 75);
                lv_obj_set_size(reverseSw, 45, 25);
                reverseSwTitle = createLabel(lv_scr_act(), reverseSw->coords.x1-20, reverseSw->coords.y1-25, 45, 30, &topInfo2, "Reversed");
                gearSetSw_06 = lv_sw_create(lv_scr_act(), NULL);
                lv_obj_set_pos(gearSetSw_06, 300+45+80, 40);
                lv_obj_set_size(gearSetSw_06, 45, 25);
                gearSetSwTitle_06 = createLabel(lv_scr_act(), gearSetSw_06->coords.x1-30, gearSetSw_06->coords.y1, 30, 30, &gear06Style, "06");
                gearSetSw_18 = lv_sw_create(lv_scr_act(), NULL);
                lv_obj_set_pos(gearSetSw_18, 300+45+80, 40+25+10);
                lv_obj_set_size(gearSetSw_18, 45, 25);
                gearSetSwTitle_18 = createLabel(lv_scr_act(), gearSetSw_18->coords.x1-30, gearSetSw_18->coords.y1, 30, 30, &gear18Style, "18");
                gearSetSw_32 = lv_sw_create(lv_scr_act(), NULL);
                lv_obj_set_pos(gearSetSw_32, 300+45+80, 75+25+10);
                lv_obj_set_size(gearSetSw_32, 45, 25);
                gearSetSwTitle_32 = createLabel(lv_scr_act(), gearSetSw_32->coords.x1-30, gearSetSw_32->coords.y1, 30, 30, &gear32Style, "32");
            }
            else if(isRotation)
            {
                reverseSw = lv_sw_create(lv_scr_act(), NULL);
                lv_obj_set_pos(reverseSw, 310, 75);
                lv_obj_set_size(reverseSw, 45, 25);
                reverseSwTitle = createLabel(lv_scr_act(), reverseSw->coords.x1-20, reverseSw->coords.y1-25, 45, 30, &topInfo2, "Reversed");

                gearSetSw_06 = lv_sw_create(lv_scr_act(), NULL);
                gearSetSw_06->hidden = true;
                gearSetSwTitle_06 = lv_label_create(lv_scr_act(), NULL);
                gearSetSwTitle_06->hidden = true;
                gearSetSw_18 = lv_sw_create(lv_scr_act(), NULL);
                gearSetSw_18->hidden = true;
                gearSetSwTitle_18 = lv_label_create(lv_scr_act(), NULL);
                gearSetSwTitle_18->hidden = true;
                gearSetSw_32 = lv_sw_create(lv_scr_act(), NULL);
                gearSetSw_32->hidden = true;
                gearSetSwTitle_32 = lv_label_create(lv_scr_act(), NULL);
                gearSetSwTitle_32->hidden = true;
            }
            else
            {
                reverseSw = lv_sw_create(lv_scr_act(), NULL);
                reverseSw->hidden = true;
                reverseSwTitle = lv_label_create(lv_scr_act(), NULL);
                reverseSwTitle->hidden = true;
                gearSetSw_06 = lv_sw_create(lv_scr_act(), NULL);
                gearSetSw_06->hidden = true;
                gearSetSwTitle_06 = lv_label_create(lv_scr_act(), NULL);
                gearSetSwTitle_06->hidden = true;
                gearSetSw_18 = lv_sw_create(lv_scr_act(), NULL);
                gearSetSw_18->hidden = true;
                gearSetSwTitle_18 = lv_label_create(lv_scr_act(), NULL);
                gearSetSwTitle_18->hidden = true;
                gearSetSw_32 = lv_sw_create(lv_scr_act(), NULL);
                gearSetSw_32->hidden = true;
                gearSetSwTitle_32 = lv_label_create(lv_scr_act(), NULL);
                gearSetSwTitle_32->hidden = true;
            }

            lv_obj_align(illiniCR, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
            returnMainMenuBtn = createBtn(lv_scr_act(), 0, 0, 125, 25, 
                &brainBTNpressed_bl, &brainBTNrelease_bl, LV_BTN_ACTION_CLICK, updateGUI, CO_BTN_MAINMENU, "Main Menu");
            lv_obj_align(returnMainMenuBtn, NULL, LV_ALIGN_IN_BOTTOM_MID, -30, 0);
            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(DISABLESCREEN):
            std::abort(); // Force a crash to stop the program
            // disableMsgImg = lv_img_create(lv_scr_act(), NULL);
            // lv_img_set_src(disableMsgImg, &disableMsg);
            // lv_obj_align(disableMsgImg, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -15);
            // // Disable robot, terminate all threads, end all tasks, disable all control, display msg to controllers
            // printf("! ! ! DISABLED ! ! !");
            // disable = true;
            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(MANUALSCREEN):
            func1 = createBtn(lv_scr_act(), gap, 20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC1, labelStrings[9].c_str());
            
            func2 = createBtn(lv_scr_act(), gap+btnwidth+gap, 20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC2, labelStrings[10].c_str());
            
            func3 = createBtn(lv_scr_act(), gap+btnwidth+gap+btnwidth+gap, 20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC3, labelStrings[11].c_str());
            
            func4 = createBtn(lv_scr_act(), gap+btnwidth+gap+btnwidth+gap+btnwidth+gap, 20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC4, labelStrings[12].c_str());
            
            func5 = createBtn(lv_scr_act(), gap, 20+40+20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC5, labelStrings[13].c_str());
            
            func6 = createBtn(lv_scr_act(), gap+btnwidth+gap, 20+40+20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC6, labelStrings[14].c_str());
            
            func7 = createBtn(lv_scr_act(), gap+btnwidth+gap+btnwidth+gap, 20+40+20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC7, labelStrings[15].c_str());
            
            func8 = createBtn(lv_scr_act(), gap+btnwidth+gap+btnwidth+gap+btnwidth+gap, 20+40+20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC8, labelStrings[16].c_str());
            
            func9 = createBtn(lv_scr_act(), gap, 20+40+20+40+20, btnwidth, 40, &manualBtnNoFunc, &manualBtnNoFunc,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC9, labelStrings[17].c_str());
            
            func10 = createBtn(lv_scr_act(), gap+btnwidth+gap, 20+40+20+40+20, btnwidth, 40, &manualBtnNoFunc, &manualBtnNoFunc,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC10, labelStrings[18].c_str());
            
            func11 = createBtn(lv_scr_act(), gap+btnwidth+gap+btnwidth+gap, 20+40+20+40+20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC11, labelStrings[19].c_str());
            
            func12 = createBtn(lv_scr_act(), gap+btnwidth+gap+btnwidth+gap+btnwidth+gap, 20+40+20+40+20, btnwidth, 40, &manualBtnPre, &manualBtnRel,
                LV_BTN_ACTION_CLICK, manualScreenFunction, MS_BTN_FUNC12, labelStrings[20].c_str());
            
            lv_obj_align(illiniCR, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
            returnMainMenuBtn = createBtn(lv_scr_act(), 0, 0, 125, 25, 
                &brainBTNpressed_bl, &brainBTNrelease_bl, LV_BTN_ACTION_CLICK, updateGUI, CO_BTN_MAINMENU, "Main Menu");
            lv_obj_align(returnMainMenuBtn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(ODOMMAP):
            currPos = createLabel(lv_scr_act(), 40, 60, 150, 30, &topInfo, "Current Pos; (xxx[X], xxx[Y])");
            heading = createLabel(lv_scr_act(), 40, 100, 150, 30, &topInfo, "Current Heading;\n(xxx.xx°[R], xxx.xx°[T])");
            // lv_img_set_src(fieldMap, &FieldMap);
            // fieldMap->hidden = false;
            // Robot Icon SetUp
            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//
        case(PORTSLIST):
            lv_obj_align(illiniCR, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
            portslistTitle = createLabel(lv_scr_act(), 0, 0, 300, 30, &topInfo, "Device and Ports List");
            lv_obj_align(portslistTitle, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
            motorsTitle = createLabel(lv_scr_act(), 30+30, 40, 150, 30, &topInfo, "Motors");
            v5DeviceTitle = createLabel(lv_scr_act(), motorsTitle->coords.x2+70, 40, 150, 30, &topInfo, "V5 Deivces");
            threeWireTitle = createLabel(lv_scr_act(), v5DeviceTitle->coords.x2+70, 40, 150, 30, &topInfo, "3 Wire");
            pl_motorList = createLabel(lv_scr_act(), motorsTitle->coords.x1-45, motorsTitle->coords.y2+3, 100, 150, &topInfo2, changingLabel[7].c_str());
            v5DeviceList = createLabel(lv_scr_act(), v5DeviceTitle->coords.x1, v5DeviceTitle->coords.y2+3, 100, 150, &topInfo2, changingLabel[8].c_str());
            threeWireList = createLabel(lv_scr_act(), threeWireTitle->coords.x1-60, threeWireTitle->coords.y2+3, 100, 150, &topInfo2, changingLabel[9].c_str());
            break;
 // >>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==>>==//

    }
}
