#include "main.h"

#define SCREEN_WIDTH_MAX 480
#define SCREEN_HEIGHT_MAX 240

static lv_obj_t *backround;
static lv_style_t plain_black;

static lv_style_t label_style;
static lv_style_t comp_auton_style;
static lv_style_t skills_auton_style;
static lv_style_t no_auton_style;
static lv_style_t brainBTNpressed;
static lv_style_t modeBtn;
static lv_style_t manualPressed;
static lv_style_t manualReleased;

static lv_obj_t *illini_label;
static lv_obj_t *robot_label;
static lv_obj_t *auton_select_label;
static lv_obj_t *comp_auton_btn;
static lv_obj_t *skills_auton_btn;
static lv_obj_t *no_auton_btn;
static lv_obj_t *tank_drive_btn;
static lv_obj_t *arcade_drive_btn;
static lv_obj_t *manual_doinker_btn;
static lv_obj_t *cata_release_btn;

LV_IMG_DECLARE(Frame);
LV_IMG_DECLARE(BothOpen);
LV_IMG_DECLARE(LeftOpen);
LV_IMG_DECLARE(RightOpen);
LV_IMG_DECLARE(BothClosed);