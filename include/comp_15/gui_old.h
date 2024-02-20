// #include <iostream>
// #include "pros/apix.h"
// #include <chrono>

// using namespace std;

// // Devices //
//     extern pros::Controller     DriverCtrler;
//     extern pros::Controller     LogisticCtrler;       
    
//     extern pros::Motor          FRTM; // Front Right Motor
//     extern pros::Motor          FRBM; // Mid Right Motor
//     extern pros::Motor          BRM; // Back Right Motor        
//     extern pros::Motor          FLTM; // Front Left Motor        
//     extern pros::Motor          FLBM; // Mid Left Motor
//     extern pros::Motor          BLM; // Back Left Motor
//     extern pros::Motor          CataR;
//     extern pros::Motor          CataL;
//     extern pros::Motor          Intake;
//     extern pros::Motor_Group    Right_Drive;
//     extern pros::Motor_Group    Left_Drive;
//     extern pros::Motor_Group    Cata;
    
//     extern pros::IMU            Gyro;
//     extern pros::Distance       WDistance;

//     extern pros::ADIButton      CataLimit;
//     extern pros::ADIEncoder     x_enc;
//     extern pros::ADIEncoder     y_enc;
//     extern pros::ADIDigitalOut  floorBrake;
//     extern pros::ADIDigitalOut  wings;
// /////////////////////////

// #define SCREEN_WIDTH_MAX 480
// #define SCREEN_HEIGHT_MAX 240
// #define RED 1
// #define BLUE 2
// #define SKILLS 3

// // Will be given by devices.cpp
// #define NUMBER_OF_MOTORS        9
// #define NUMBER_OF_SENSORS       2
// #define NUMBER_OF_SINGLE3WIRE   3
// #define NUMBER_OF_DUAL3WIRE     2

// typedef enum {
//     // Common Objects //
//     CO_BTN_MAINMENU,
//     CO_IMG_ETHEREALLOGO,
//     CO_IMG_FIELDMAP,
//     // MAINMENU //
//     MM_BTN_ILLINILOGO,
//     MM_BTN_SETAUTON,
//     MM_BTN_SETTING,
//     MM_BTN_DEVICECHECK,
//     MM_BTN_PORTCHANGER,
//     MM_BTN_MANUALCTRL,
//     MM_BTN_ODOMMAP,
//     MM_BTNLABEL_BATTERYINFO,
//     MM_BTNLABEL_SDCARD,
//     MM_BTNLABEL_ROBOTTYPE,
//     MM_BTN_ERRORBOX,
//     // TEAMINFO //
//     TI_BTNLABEL_TEAMINFOTITLE,
//     TI_LABEL_ETHEREALPROGRAMVERSION,
//     TI_LABEL_TEAMNAMES,                 //15
//     // SETAUTONMAIN //
//     SA_LABEL_CHOOSETEXT,                //16
//     SA_BTN_MATCHAUTON=18,               //18
//     SA_BTN_SKILLS,
//     SA_LINE_SKILLS,
//     SA_BTN_AUTONONE,
//     SA_LINE_AUTONONE,
//     SA_BTN_AUTONTWO,
//     SA_LINE_AUTONTWO,
//     SA_BTN_AUTONTHREE,
//     SA_LINE_AUTONTHREE,
//     SA_BTN_AUTONFOUR,
//     SA_LINE_AUTONFOUR,
//     SA_BTN_AUTONFIVE,
//     SA_LINE_AUTONFIVE,
//     SA_BTN_AUTONSIX,
//     SA_LINE_AUTONSIX,
//     // SETTINGS //
//     S_BTN_INERTIALCALI,
//     S_LABEL_GYROINFO,
//     S_IMG_CALIBRATESTATE,
//     S_BTN_MANUALAUTON,
//     S_LABEL_MANUALAUTONSTATE,
//     S_BTN_MINESTRONE,
//     S_BTN_RAZE,
//     S_BTNLABEL_FLYWHEELSPEED,
//     S_LABEL_FLYWHEELSPEEDVOLTAGE,
//     S_SLIDER_FLYWHEELSPEED,
//     S_BTN_BATTERYSAVER,
//     // RAZE_SS // 
//     RSS_IMG_FRAME,
//     RSS_IMG_RAZEFACE,
//     // DEVICECHECK //
//     TYPE_MOTOR=0,
//     TYPE_SENSOR,
//     TYPE_ADI,
//     DC_BTNLABEL_FRTM=0,
//     DC_BTNLABEL_FRBM,
//     DC_BTNLABEL_BRM,
//     DC_BTNLABEL_FLTM,
//     DC_BTNLABEL_FLBM,
//     DC_BTNLABEL_BLM,
//     DC_BTNLABEL_INTAKE,
//     DC_BTNLABEL_CATAR,
//     DC_BTNLABEL_CATAL,
//     DC_BTNLABEL_IMU=0,
//     DC_BTNLABEL_DISTANCE,
//     DC_BTNLABEL_LIMIT=0,
//     DC_BTNLABEL_XENCODER,
//     DC_BTNLABEL_YENCODER,
//     DC_BTNLABEL_FB_PNEU,
//     DC_BTNLABEL_WINGS_PNEU,
//     // DC_BTNLABEL_MOTORTITLE,
//     // DC_BTNLABEL_SENSORTITLE,
//     // DC_BTNLABEL_ADITITLE,
//     // DC_LABEL_MOTORLIST,
//     // DC_LABEL_SENSORLIST,
//     // DC_LABEL_ADILIST,
//     // PORTCHANGE //
//     PC_ROLLER_DEVICEROL=64,
//     PC_ROLLER_V5_PORTROL,
//     PC_BTN_CONFIRMCHANGEBTN,
//     PC_BTN_RESETDEFBTN,
//     PC_LABEL_DEVICECUSTOMTITLE,
//     PC_SWITCH_REVERSESW,
//     PC_LABLE_REVERSESWTITLE,
//     PC_SWITCH_GEARSETSW_06,
//     PC_SWITCH_GEARSETSW_18,
//     PC_SWITCH_GEARSETSW_32,
//     PC_LABLE_GEARSETSWSWTITLE_06,
//     PC_LABLE_GEARSETSWSWTITLE_18,
//     PC_LABLE_GEARSETSWSWTITLE_32,
//     // DISABLESCREEN //
//     DS_IMG_DISABLEMSGIMG,
//     // MANUALSCREEN //
//     MS_BTN_FUNC1,
//     MS_BTN_FUNC2,
//     MS_BTN_FUNC3,
//     MS_BTN_FUNC4,
//     MS_BTN_FUNC5,
//     MS_BTN_FUNC6,
//     MS_BTN_FUNC7,
//     MS_BTN_FUNC8,
//     MS_BTN_FUNC9,
//     MS_BTN_FUNC10,
//     MS_BTN_FUNC11,
//     MS_BTN_FUNC12,
//     // ODOMMAP //
//     OM_LABEL_CURRPOS,
//     OM_LABEL_HEADING,
//     // PORTSLIST //
//     PL_LABEL_PORTSLISTTITLE,
//     PL_LABEL_MOTORSTITLE,
//     PL_LABEL_V5DEIVCETITLE,
//     PL_LABEL_THREEWIRETITLE,
//     PL_LABEL_MOTORLIST,
//     PL_LABEL_V5DEIVCELIST,
//     PL_LABEL_THREEWIRELIST
// } gui_btn_unique_id;

// #define AUTON_RED_POINTS 0
// #define AUTON_BLUE_POINTS 1
// #define AUTON_SKILLS_POINTS 2

// // Common Objects //
//     static lv_obj_t *illiniCR;
//     static lv_obj_t *backround;
//     static lv_obj_t *illiniLogo;
//     static lv_obj_t *fieldMap;
//     static lv_obj_t *returnMainMenuBtn;
//     static lv_obj_t *robudIcon;
//     static lv_obj_t *robudLine;
//     static lv_style_t robudIconStyle;
//     static lv_style_t robudLineStyle;
//     static lv_style_t brainBTNrelease;
//     static lv_style_t brainBTNpressed;
//     static lv_style_t brainBTNrelease_og;
//     static lv_style_t brainBTNpressed_og;
//     static lv_style_t brainBTNrelease_bl;
//     static lv_style_t brainBTNpressed_bl;
//     static lv_style_t plain_black;
//     static lv_style_t topInfo; // White outline box
//     static lv_style_t topInfo2; // No box
// // End of Common obj //

// // Img Declares //
//     LV_IMG_DECLARE(simpleIlliniLogo);
//     LV_IMG_DECLARE(IlliniLogoMain);
//     LV_IMG_DECLARE(FieldMapLarge);
//     LV_IMG_DECLARE(FieldMapSmall);
//     LV_IMG_DECLARE(Frame);
//     LV_IMG_DECLARE(BothOpen);
//     LV_IMG_DECLARE(LeftOpen);
//     LV_IMG_DECLARE(RightOpen);
//     LV_IMG_DECLARE(BothClosed);
//     LV_IMG_DECLARE(downArrow); // To remove
//     LV_IMG_DECLARE(upArrow);    // To remove
// // End of Img declares //

// // Screen Defines w/Objects //
// #define LOGOSCREEN      0
//     // Uses etherealLogo
// #define MAINMENU        1
//     static lv_obj_t *illiniLogoBtn;
//     static lv_obj_t *setAutonBtn;
//     static lv_obj_t *settingBtn;
//     static lv_obj_t *deviceCheckBtn;
//     static lv_obj_t *PortChangerBtn;
//     static lv_obj_t *manualCtrlBtn;
//     static lv_obj_t *odomMapBtn;
//     static lv_obj_t *batteryInfo;
//     static lv_obj_t *sdcard;
//     static lv_obj_t *robotType;
//     static lv_obj_t *errorBox;
//     static lv_style_t errorBoxStyle;
// #define TEAMINFO        2
//     // Uses IlliniLogoMain logo
// #define SETAUTON        3
//     static lv_point_t autonLinePoints[3][9];
//     static int16_t inputX;
//     static int16_t inputY;
//     static lv_point_t rbudHeading[2];
//     static lv_obj_t *chooseLable;
//     static lv_obj_t *matchAuton;
//     static lv_obj_t *skills;
//     static lv_obj_t *autonOne;
//     static lv_obj_t *autonTwo;
//     static lv_obj_t *autonThree;
//     static lv_obj_t *autonFour;
//     static lv_obj_t *autonFive;
//     static lv_obj_t *autonSix;
//     static lv_obj_t *blueside_line;
//     static lv_obj_t *redside_line;
//     static lv_obj_t *skills_line;
//     static lv_style_t customLable;
//     static lv_style_t matchAutonStyle;
//     static lv_style_t matchAutonStyle_tog;
//     static lv_style_t skillsStyle;
//     static lv_style_t skillsStyle_tog;
//     static lv_style_t autonBtnStylePr;
//     static lv_style_t autonBtnStyleRel;
//     static lv_style_t noAutonBtnStyle;
//     static lv_style_t lineStyle_Red;
//     static lv_style_t lineStyle_Blue;
//     static lv_style_t lineStyle_Skills;
//     // Uses robudIcon
//     // Uses robudLine
// #define SETTINGS        4
//     static lv_obj_t *inertialCaliBtn;
//     static lv_obj_t *gyroInfo;
//     static lv_obj_t *calibrateState;
//     static lv_obj_t *manualAutonBtn;
//     static lv_obj_t *manualAutonStateLabel;
//     static lv_obj_t *razeBtn;
//     static lv_obj_t *driverProfileDD;
//     // static lv_obj_t *flywheelSpeedBtn;
//     // static lv_obj_t *flywheelSpeedLabel;
//     // static lv_obj_t *flywheelSpeed;
//     // static lv_obj_t *intakeSpeedBtn;
//     // static lv_obj_t *intakeSpeedLabel;
//     // static lv_obj_t *intakeSpeed;
//     static lv_style_t razeCustomStyle;
//     static lv_style_t razeCustomStylePre;
//     static lv_style_t sliderKnobStyle;
//     static lv_style_t sliderIndicStyle;
//     static lv_style_t dropdownStyle;
// #define RAZE_SS         5
//     static lv_obj_t *frameObj;
//     static lv_obj_t *razeImg;
// #define DEVICECHECK     6
//     // None of these buttons can be pressed; Convert to label
//     static lv_obj_t *frtm_btn;
//     static lv_obj_t *frbm_btn;
//     static lv_obj_t *brm_btn;
//     static lv_obj_t *fltm_btn;
//     static lv_obj_t *flbm_btn;
//     static lv_obj_t *blm_btn;
//     static lv_obj_t *intake_btn;
//     static lv_obj_t *cataR_btn;
//     static lv_obj_t *cataL_btn;
//     static lv_obj_t *imu_btn;
//     static lv_obj_t *distance_btn;
//     static lv_obj_t *limit_btn;
//     static lv_obj_t *x_encoder_btn;
//     static lv_obj_t *y_encoder_btn;
//     static lv_obj_t *fb_pneu_btn;
//     static lv_obj_t *wings_pneu_btn;
//     // static lv_obj_t *motorBtnTitle;
//     // static lv_obj_t *sensorBtnTitle;
//     // static lv_obj_t *ADIBtnTitle;
//     // static lv_obj_t *dc_motorList;
//     // static lv_obj_t *sensorList;
//     // static lv_obj_t *ADIList;
//     static lv_style_t offline;
//     static lv_style_t hot;
//     static lv_style_t online;
//     static lv_style_t unknown;
//     static lv_style_t lineStyle2;
// #define PORTCHANGE      7
//     static lv_obj_t *deviceRol;
//     static lv_obj_t *V5_portRol;
//     static lv_obj_t *confirmChangeBtn;
//     static lv_obj_t *resetDefBtn;
//     static lv_obj_t *deviceCustomTitle;
//     static lv_obj_t *reverseSw;
//     static lv_obj_t *reverseSwTitle;
//     static lv_obj_t *gearSetSw_06;
//     static lv_obj_t *gearSetSw_18;
//     static lv_obj_t *gearSetSw_32;
//     static lv_obj_t *gearSetSwTitle_06;
//     static lv_obj_t *gearSetSwTitle_18;
//     static lv_obj_t *gearSetSwTitle_32;
//     static lv_style_t gear06Style;
//     static lv_style_t gear18Style;
//     static lv_style_t gear32Style;
// #define DISABLESCREEN   8
//     // static lv_obj_t *disableMsgImg;  // Lable
// #define MANUALSCREEN    9
//     static lv_obj_t *func1;
//     static lv_obj_t *func2;
//     static lv_obj_t *func3;
//     static lv_obj_t *func4;
//     static lv_obj_t *func5;
//     static lv_obj_t *func6;
//     static lv_obj_t *func7;
//     static lv_obj_t *func8;
//     static lv_obj_t *func9;
//     static lv_obj_t *func10;
//     static lv_obj_t *func11;
//     static lv_obj_t *func12;
//     static lv_style_t manualBtnRel;
//     static lv_style_t manualBtnPre;
//     static lv_style_t manualBtnNoFunc;
// #define ODOMMAP         10
//     // Return Menu moves Position to Top Left
//     static lv_obj_t *currPos;
//     static lv_obj_t *heading;
//     // Uses field map -> Switch source to larger map
//     // Uses robudIcon
//     // static lv_style_t redOutline;
//     // static lv_style_t blueOutline;
// #define PORTSLIST       11
//     static lv_obj_t *portslistTitle;
//     static lv_obj_t *motorsTitle;
//     static lv_obj_t *v5DeviceTitle;
//     static lv_obj_t *threeWireTitle;
//     static lv_obj_t *pl_motorList;
//     static lv_obj_t *v5DeviceList;
//     static lv_obj_t *threeWireList;
//     static string motorListStr;
//     static string v5DeviceListStr;
//     static string threeWireListStr;

// // End of static obj list //

// class brain_gui
// {
//     public:
//     static int lastScreen;
//     static int currentScreen;
//     static bool disable;
//     static uint8_t id;
//     static bool updated;

//     static int autonColor;
//     static int autonType;
    
//     static int inertialStat;
//     inline static bool startUpInit = true;
    
//     inline static float sliderPos_fly = 6.25;       // Replace with const
//     inline static float sliderPos_intake = 5.25;    // Replace with const
//     inline static short int driverProfile = 2;
//     inline static double razeBlickDelay = 2.5;      // Replace with const
//     inline static int btnwidth = 105;               // Replace with const
//     inline static int gap = 12;                     // Replace with const
//     static char sliderPos_fly_str[14];
//     static char sliderPos_intake_str[14];

//     static bool runRaze;
//     static bool glitch;
//     static float delayCountOne;
//     static bool isLogo;
//     static pros::screen_touch_status_s_t lastClick;

//     static bool isDeviceSelected;
//     static short int deviceSelected;
//     static bool isV5Port;
//     static bool isMotor;
//     static bool isRotation;

//     static bool deviceConnectionError;

//     static chrono::high_resolution_clock::time_point guiLastUpdate;
//     static chrono::high_resolution_clock::time_point guiCurrentTime;
//     static int update_TimeEslaped;

//     inline static std::string nameTemp = "";


//     // static lv_obj_t *createBtn(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
//     //     lv_style_t *style_pressed, lv_style_t *style_released, int id);
//     static lv_obj_t *createBtn(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
//     lv_style_t *style_pressed, lv_style_t *style_released, lv_btn_action_t actionTrig, lv_action_t btnAction, int id, const char *title);
//     // static lv_obj_t *createBtn(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
//     // lv_style_t *style_pressed, lv_style_t *style_released, int id, const char *title);
//     static void robotIcon_init();
//     static void robotIcon_update(int xPos, int yPos, float heading);
//     static void robotIcon_updateHidden(bool isHidden);
//     static lv_res_t updateGUI(lv_obj_t *btn);
//     static void updateLabel();
//     static lv_obj_t *createLabel(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
//         lv_style_t *style, const char * title);
//     static void deviceCheck_motorStyle_update(lv_obj_t *btn);
//     static void runMainGUI();
//     static lv_res_t updateAutonColor(lv_obj_t *btn);
//     static lv_res_t updateAutonType(lv_obj_t *Inp);
//     static lv_res_t flySpeedUpdate(lv_obj_t *Inp);
//     static lv_res_t saveLastDriveProfile(lv_obj_t *btn);
//     static lv_res_t manualScreenFunction(lv_obj_t *btn);
//     static lv_res_t portScreen_roller_update(lv_obj_t *inpObj);
//     static void lastScreen_del(int screen=lastScreen);
//     static void raze_ss_runner();
//     static lv_res_t intakeSpeedUpdate(lv_obj_t *Inp);

//     public:
//     brain_gui();
//     static void lvglGUIInit();
//     static void guiControlLoop();
// };

// /*
// Things to Change with final integration
// =========================================
// - (HOLD) In init method, update device list string generator to read from deviceList
// - Set Up manual trigger of auton in settings
// - Program manual auton features
// - Link device changer with device file
// - Link Manual control functions
// - GUI Update Log
// - Motor Update Log
// - Robot Type Updater
// */



// /*
// - X Pos < Short Int < 2B
// - Y Pos < Short Int < 2B
// - Heading < float < 4B
// - Device Disconnects
// - All Motor Current RPM + Direction
// - Device Config as Seperate Log
// - GUI Updates as Seperate Log
// - Optical Hue
// - Distance
// - Rotation Positions

// 72 Bytes
// When storing data, total up data into a seperate file to get total data stored
// */