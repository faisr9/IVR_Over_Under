#pragma once
#include "dookie/devices.h"
#include "dookie/include_list.h"

#define SCREEN_WIDTH_MAX 480
#define SCREEN_HEIGHT_MAX 240

static lv_obj_t *backround;
static lv_style_t plain_black;

static lv_style_t label_style;
static lv_style_t comp_auton_style;
static lv_style_t skills_auton_style;
static lv_style_t no_auton_style;
static lv_style_t comp_auton_style_sel;
static lv_style_t skills_auton_style_sel;
static lv_style_t no_auton_style_sel;
static lv_style_t brainBTNpressed;
static lv_style_t modeBtnStyle;
static lv_style_t manualPressed;
static lv_style_t manualReleased;
static lv_style_t switch_knob_off_style;
static lv_style_t switch_knob_on_style;
static lv_style_t switch_bg_style;
static lv_style_t switch_indic_style;

static lv_obj_t *illini_label;
static lv_obj_t *robot_label;
static lv_obj_t *auton_select_label;
static lv_obj_t *comp_auton_btn;
static lv_obj_t *skills_auton_btn;
static lv_obj_t *no_auton_btn;
static lv_obj_t *tank_drive_btn;
static lv_obj_t *tank_drive_switch;
static lv_obj_t *arcade_drive_btn;
static lv_obj_t *arcade_drive_switch;
static lv_obj_t *manual_trigger_btn1;
static lv_obj_t *manual_trigger_btn2;

static lv_obj_t *frameObj;
static lv_obj_t *razeImg;

static double razeBlickDelay;
static bool runRaze;
static bool glitch;
static float delayCountOne;
static pros::screen_touch_status_s_t lastClick;
static int screenSave_delay;

class gui {
    public:
        typedef enum autonomous_type {
            AUTON_COMP,
            AUTON_SKILLS,
            AUTON_NONE,
            NO_SELECTION
        } autonomous_type_t;

        inline static autonomous_type_t selected_auton = autonomous_type_t::NO_SELECTION;
        inline static bool tank_drive = false;
        static void gui_init();

        // static pros::Task *raze_ss;
};

LV_IMG_DECLARE(Frame);
LV_IMG_DECLARE(BothOpen);
LV_IMG_DECLARE(LeftOpen);
LV_IMG_DECLARE(RightOpen);
LV_IMG_DECLARE(BothClosed);