#include "common_code/raze.h"

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
lv_obj_t* createBtn(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
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
lv_obj_t* createLabel(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
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

void razeinit()
{
    lv_style_copy(&plain_black, &lv_style_plain);
    plain_black.body.main_color = LV_COLOR_BLACK;
    plain_black.body.grad_color = LV_COLOR_BLACK;
    plain_black.body.radius = 0;

    lv_style_copy(&modeBtn, &lv_style_plain);
    modeBtn.body.main_color = LV_COLOR_MAKE(72, 139, 247);
    modeBtn.body.grad_color = LV_COLOR_MAKE(72, 139, 247);
    modeBtn.body.radius = 1;
    modeBtn.text.color = LV_COLOR_MAKE(255, 255, 255);

    lv_style_copy(&label_style, &lv_style_plain);
    label_style.body.main_color = LV_COLOR_BLACK;
    label_style.body.grad_color = LV_COLOR_BLACK;
    label_style.body.border.width = 2;
    label_style.body.border.color = LV_COLOR_BLACK;
    label_style.body.radius = 2;
    label_style.text.color = LV_COLOR_WHITE;
}
