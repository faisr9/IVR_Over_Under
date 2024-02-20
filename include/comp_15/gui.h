#pragma once
#include "comp_15/devices.h"
#include "comp_15/include_list.h"

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
static lv_style_t switchStyle;

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
static lv_obj_t *manual_doinker_btn;
static lv_obj_t *manual_doinker_switch;
static lv_obj_t *manual_trigger_btn1;
static lv_obj_t *manual_trigger_btn2;

namespace gui {
    typedef enum autonomous_type {
        AUTON_COMP,
        AUTON_SKILLS,
        AUTON_NONE
    } autonomous_type_t;

    static autonomous_type_t selected_auton = autonomous_type_t::AUTON_NONE;
    static bool tank_drive = false;
    static bool manual_doinker_enable = false;

} // namespace gui
void gui_init();

LV_IMG_DECLARE(Frame);
LV_IMG_DECLARE(BothOpen);
LV_IMG_DECLARE(LeftOpen);
LV_IMG_DECLARE(RightOpen);
LV_IMG_DECLARE(BothClosed);