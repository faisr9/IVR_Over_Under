#pragma once
#include "pookie/devices.h"
#include "pookie/include_list.h"

#define SCREEN_WIDTH_MAX 480
#define SCREEN_HEIGHT_MAX 240

static lv_obj_t *backround;
static lv_style_t plain_black;

static lv_style_t label_style;
static lv_style_t wp_auton_style;
static lv_style_t elim_auton_style;
static lv_style_t wp_auton_style_sel;
static lv_style_t elim_auton_style_sel;
static lv_style_t brainBTNpressed;
static lv_style_t modeBtnStyle;
static lv_style_t manualPressed;
static lv_style_t manualReleased;

static lv_obj_t *illini_label;
static lv_obj_t *robot_label;
static lv_obj_t *auton_select_label;
static lv_obj_t *wp_auton_btn;
static lv_obj_t *elim_auton_btn;
static lv_obj_t *manual_trigger_btn1;

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
            AUTON_WP,
            AUTON_ELIM,
            NO_SELECTION
        } autonomous_type_t;

        inline static autonomous_type_t selected_auton = autonomous_type_t::NO_SELECTION;
        static void gui_init();
};

LV_IMG_DECLARE(Frame);
LV_IMG_DECLARE(BothOpen);
LV_IMG_DECLARE(LeftOpen);
LV_IMG_DECLARE(RightOpen);
LV_IMG_DECLARE(BothClosed);