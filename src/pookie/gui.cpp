#include "pookie/gui.h"

pros::Task *raze_ss;

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

lv_res_t autonSelection(lv_obj_t *btn)
{
    short int id = lv_obj_get_free_num(btn);
    screenSave_delay = 0;
    
    switch(id)
    {
        case(1):
            lv_btn_set_state(wp_auton_btn, LV_BTN_STATE_PR);
            lv_btn_set_state(elim_auton_btn, LV_BTN_STATE_REL);
            gui::selected_auton = gui::autonomous_type::AUTON_WP;
            break;
        case(2):
            lv_btn_set_state(wp_auton_btn, LV_BTN_STATE_REL);
            lv_btn_set_state(elim_auton_btn, LV_BTN_STATE_PR);
            gui::selected_auton = gui::autonomous_type::AUTON_ELIM;
            break;
    }

    return LV_RES_OK;
}

lv_res_t manualTrigger(lv_obj_t *btn)
{
    short int id = lv_obj_get_free_num(btn);
    screenSave_delay = 0;

    switch (id)
    {
        case 1:
            lv_btn_set_state(btn, LV_BTN_STATE_PR);
            imu.reset(true);
            lv_btn_set_state(btn, LV_BTN_STATE_REL);
            break;
    }

    return LV_RES_OK;
}

void raze_ss_runner()
{
    frameObj = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(frameObj, &Frame);
    lv_obj_align(frameObj, NULL, LV_ALIGN_CENTER, 0, 0);
    razeImg = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(razeImg, &BothOpen);
    lv_obj_align(razeImg, NULL, LV_ALIGN_CENTER, 0, 0);
    runRaze = true;

    razeImg->hidden = true;
    frameObj->hidden = true;
    
    pros::delay(1000); // Delay to allow the screen to be fully initialized
    
    screenSave_delay = 0;

    while(1)
    {
        if (screenSave_delay > 15 || (pros::competition::is_connected() && !pros::competition::is_disabled()))
        {
            screenSave_delay = 0;
            lv_obj_del(illini_label);
            lv_obj_del(robot_label);
            lv_obj_del(auton_select_label);
            lv_obj_del(manual_trigger_btn1);

            frameObj->hidden = false;
            razeImg->hidden = false;

            pros::delay(1000); // Delay to allow the screen to be fully initialized
            lv_obj_align(frameObj, NULL, LV_ALIGN_CENTER, 10, 0);
            delay(150);
            lv_obj_align(frameObj, NULL, LV_ALIGN_CENTER, 0, 0);

            if(pros::competition::is_connected() && !pros::competition::is_disabled())
                glitch = true;
            else
                glitch = false;

            break;
        }
        else
        {
            screenSave_delay++;
        }

        pros::delay(1000);
    }

    while(runRaze)
    {
        if(pros::competition::is_connected() && !pros::competition::is_disabled())
            glitch = true;
        else
            glitch = false;

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
                lv_obj_align(razeImg, NULL, LV_ALIGN_CENTER, 0, 0);
                delayCountOne += 0.05;
                pros::Task::delay(50);
            }
        }

        if(lastClick.touch_status == pros::E_TOUCH_HELD)
        {
            runRaze = false;      
            lv_obj_del(frameObj);
            lv_obj_del(razeImg);

            Task ss_restart(gui::gui_init);
        }
    }
}

void gui::gui_init()
{
    if (pros::lcd::is_initialized())
        pros::lcd::shutdown();

    delay(200);
    lv_init();

    // Create Styles //
    lv_style_copy(&plain_black, &lv_style_plain);
    plain_black.body.main_color = LV_COLOR_BLACK;
    plain_black.body.grad_color = LV_COLOR_BLACK;
    plain_black.body.radius = 0;

    lv_style_copy(&modeBtnStyle, &lv_style_plain);
    modeBtnStyle.body.main_color = LV_COLOR_MAKE(72, 139, 247);
    modeBtnStyle.body.grad_color = LV_COLOR_MAKE(72, 139, 247);
    modeBtnStyle.body.radius = 1;
    modeBtnStyle.text.color = LV_COLOR_MAKE(255, 255, 255);

    lv_style_copy(&label_style, &lv_style_plain);
    label_style.body.main_color = LV_COLOR_BLACK;
    label_style.body.grad_color = LV_COLOR_BLACK;
    label_style.body.border.width = 2;
    label_style.body.border.color = LV_COLOR_BLACK;
    label_style.body.radius = 2;
    label_style.text.color = LV_COLOR_WHITE;

    lv_style_copy(&wp_auton_style, &lv_style_plain);
    wp_auton_style.body.main_color = LV_COLOR_MAKE(255-150, 0, 0);
    wp_auton_style.body.grad_color = LV_COLOR_MAKE(255-150, 0, 0);
    wp_auton_style.body.radius = 1;
    wp_auton_style.text.color = LV_COLOR_MAKE(205-80, 205-80, 205-80);

    lv_style_copy(&elim_auton_style, &wp_auton_style);
    elim_auton_style.body.main_color = LV_COLOR_MAKE(0, 0, 255-150);
    elim_auton_style.body.grad_color = LV_COLOR_MAKE(0, 0, 255-150);

    lv_style_copy(&wp_auton_style_sel, &lv_style_plain);
    wp_auton_style_sel.body.main_color = LV_COLOR_MAKE(255, 0, 0);
    wp_auton_style_sel.body.grad_color = LV_COLOR_MAKE(255, 0, 0);
    wp_auton_style_sel.body.radius = 1;
    wp_auton_style_sel.text.color = LV_COLOR_WHITE;

    lv_style_copy(&elim_auton_style_sel, &wp_auton_style_sel);
    elim_auton_style_sel.body.main_color = LV_COLOR_MAKE(0, 0, 255);
    elim_auton_style_sel.body.grad_color = LV_COLOR_MAKE(0, 0, 255);

    lv_style_copy(&manualReleased, &lv_style_plain);
    manualReleased.body.main_color = LV_COLOR_MAKE(238, 238, 238);
    manualReleased.body.grad_color = LV_COLOR_MAKE(238, 238, 238);
    manualReleased.body.border.color = LV_COLOR_MAKE(24, 84, 182);
    manualReleased.body.border.width = 2;
    manualReleased.text.color = LV_COLOR_BLACK;
    manualReleased.body.radius = 2;

    lv_style_copy(&manualPressed, &lv_style_plain);
    manualPressed.body.main_color = LV_COLOR_MAKE(145, 145, 145);
    manualPressed.body.grad_color = LV_COLOR_MAKE(145, 145, 145);
    manualPressed.body.border.color = LV_COLOR_MAKE(24, 84, 182);
    manualPressed.body.border.width = 2;
    manualPressed.body.radius = 8;

    // Create Objects //
    backround = createBtn(lv_scr_act(), 0, 0, SCREEN_WIDTH_MAX, SCREEN_HEIGHT_MAX,
            &plain_black, &plain_black, LV_BTN_ACTION_LONG_PR_REPEAT, NULL, 0, "");

    illini_label = createLabel(lv_scr_act(), 0, 0, 0, 0, &label_style, "ILL-INI");
    lv_obj_align(illini_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    robot_label = createLabel(lv_scr_act(), 0, 0, 0, 0, &label_style, "POOKIE");
    lv_obj_align(robot_label, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

    auton_select_label = createLabel(lv_scr_act(), 30, 35, 0, 0, &label_style, "Choose Auton:");
    lv_obj_align(auton_select_label, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 10);

    wp_auton_btn = createBtn(lv_scr_act(), 45, 65, 100, 35, 
        &wp_auton_style_sel, &wp_auton_style, LV_BTN_ACTION_CLICK, autonSelection, 1, "WinPoint");
    lv_btn_set_state(wp_auton_btn, LV_BTN_STATE_REL);
    lv_obj_align(wp_auton_btn, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 40);

    elim_auton_btn = createBtn(lv_scr_act(), 45, 65+35+10, 100, 35, 
        &elim_auton_style_sel, &elim_auton_style, LV_BTN_ACTION_CLICK, autonSelection, 2, "Elims");
    lv_btn_set_state(elim_auton_btn, LV_BTN_STATE_REL);
    lv_obj_align(elim_auton_btn, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 40+35+10);

    if (gui::selected_auton != gui::autonomous_type::NO_SELECTION)
    {
        switch(gui::selected_auton)
        {
            case(gui::autonomous_type::AUTON_WP):
                lv_btn_set_state(wp_auton_btn, LV_BTN_STATE_PR);
                break;
            case(gui::autonomous_type::AUTON_ELIM):
                lv_btn_set_state(elim_auton_btn, LV_BTN_STATE_PR);
                break;
        }
    }

    manual_trigger_btn1 = createBtn(lv_scr_act(), 0, 0, 130, 35,
        &manualPressed, &manualReleased, LV_BTN_ACTION_CLICK, manualTrigger, 1, "IMU Reset");
    lv_obj_align(manual_trigger_btn1, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 160);

    raze_ss = new pros::Task([=] {raze_ss_runner();}, "Raze SS Task");
    raze_ss->set_priority(6);
}
