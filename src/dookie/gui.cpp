#include "dookie/gui.h"

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
            lv_btn_set_state(comp_auton_btn, LV_BTN_STATE_PR);
            lv_btn_set_state(skills_auton_btn, LV_BTN_STATE_REL);
            // lv_btn_set_state(no_auton_btn, LV_BTN_STATE_REL);
            gui::selected_auton = gui::autonomous_type::AUTON_COMP;
            break;
        case(2):
            lv_btn_set_state(comp_auton_btn, LV_BTN_STATE_REL);
            lv_btn_set_state(skills_auton_btn, LV_BTN_STATE_PR);
            // lv_btn_set_state(no_auton_btn, LV_BTN_STATE_REL);
            gui::selected_auton = gui::autonomous_type::AUTON_SKILLS;
            break;
        case(3):
            lv_btn_set_state(comp_auton_btn, LV_BTN_STATE_REL);
            lv_btn_set_state(skills_auton_btn, LV_BTN_STATE_REL);
            // lv_btn_set_state(no_auton_btn, LV_BTN_STATE_PR);
            gui::selected_auton = gui::autonomous_type::AUTON_NONE;
            break;
    }

    return LV_RES_OK;
}

lv_res_t driveTypeUpdate(lv_obj_t *btn_inp)
{
    short int id = lv_obj_get_free_num(btn_inp);
    screenSave_delay = 0;

    switch (id)
    {
        case 1:
            lv_sw_on_anim(tank_drive_switch);
            lv_sw_off_anim(arcade_drive_switch);
            gui::tank_drive = true;
            // drive.change_drive_mode(1);
            break;
        case 2:
            lv_sw_on_anim(arcade_drive_switch);
            lv_sw_off_anim(tank_drive_switch);
            gui::tank_drive = false;
            // drive.change_drive_mode(0);
            break;
    }

    // printf("Tank Drive: %s\n", gui::tank_drive ? "True" : "False");

    return LV_RES_OK;   
}

extern void switch_to_gui();
lv_res_t manualTrigger(lv_obj_t *btn)
{
    short int id = lv_obj_get_free_num(btn);
    screenSave_delay = 0;

    switch (id)
    {
        case 1:
            // Delete all objects
            lv_obj_del(illini_label);
            lv_obj_del(robot_label);
            lv_obj_del(auton_select_label);
            lv_obj_del(comp_auton_btn);
            lv_obj_del(skills_auton_btn);
            // lv_obj_del(no_auton_btn);
            lv_obj_del(tank_drive_btn);
            lv_obj_del(tank_drive_switch);
            lv_obj_del(arcade_drive_btn);
            lv_obj_del(arcade_drive_switch);
            lv_obj_del(manual_trigger_btn1);
            lv_obj_del(manual_trigger_btn2);

            raze_ss->remove();

            pros::lcd::initialize();
            lcd::register_btn2_cb(switch_to_gui);
            break;
        case 2:
            Pneumatics::getInstance()->getWings()->off();
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
            lv_obj_del(comp_auton_btn);
            lv_obj_del(skills_auton_btn);
            // lv_obj_del(no_auton_btn);
            lv_obj_del(tank_drive_btn);
            lv_obj_del(tank_drive_switch);
            lv_obj_del(arcade_drive_btn);
            lv_obj_del(arcade_drive_switch);
            lv_obj_del(manual_trigger_btn1);
            lv_obj_del(manual_trigger_btn2);

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

    gui::tank_drive = false;

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

    lv_style_copy(&comp_auton_style, &lv_style_plain);
    comp_auton_style.body.main_color = LV_COLOR_MAKE(255-150, 0, 0);
    comp_auton_style.body.grad_color = LV_COLOR_MAKE(255-150, 0, 0);
    comp_auton_style.body.radius = 1;
    comp_auton_style.text.color = LV_COLOR_MAKE(205-80, 205-80, 205-80);

    lv_style_copy(&skills_auton_style, &comp_auton_style);
    skills_auton_style.body.main_color = LV_COLOR_MAKE(0, 0, 255-150);
    skills_auton_style.body.grad_color = LV_COLOR_MAKE(0, 0, 255-150);

    lv_style_copy(&no_auton_style, &comp_auton_style);
    no_auton_style.body.main_color = LV_COLOR_MAKE(205-80, 0, 0);
    no_auton_style.body.grad_color = LV_COLOR_MAKE(205-80, 0, 0);

    lv_style_copy(&comp_auton_style_sel, &lv_style_plain);
    comp_auton_style_sel.body.main_color = LV_COLOR_MAKE(255, 0, 0);
    comp_auton_style_sel.body.grad_color = LV_COLOR_MAKE(255, 0, 0);
    comp_auton_style_sel.body.radius = 1;
    comp_auton_style_sel.text.color = LV_COLOR_WHITE;

    lv_style_copy(&skills_auton_style_sel, &comp_auton_style_sel);
    skills_auton_style_sel.body.main_color = LV_COLOR_MAKE(0, 0, 255);
    skills_auton_style_sel.body.grad_color = LV_COLOR_MAKE(0, 0, 255);

    lv_style_copy(&no_auton_style_sel, &comp_auton_style_sel);
    no_auton_style_sel.body.main_color = LV_COLOR_MAKE(255, 0, 0);
    no_auton_style_sel.body.grad_color = LV_COLOR_MAKE(255, 0, 0);

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

    lv_style_copy(&switch_knob_off_style, &lv_style_plain);
    switch_knob_off_style.body.main_color = LV_COLOR_MAKE(100, 0, 0);
    switch_knob_off_style.body.grad_color = LV_COLOR_MAKE(100, 0, 0);
    switch_knob_off_style.body.radius = 15;

    lv_style_copy(&switch_knob_on_style, &lv_style_plain);
    switch_knob_on_style.body.main_color = LV_COLOR_MAKE(0, 0, 255);
    switch_knob_on_style.body.grad_color = LV_COLOR_MAKE(0, 0, 255);
    switch_knob_on_style.body.radius = 15;

    lv_style_copy(&switch_bg_style, &lv_style_plain);
    switch_bg_style.body.main_color = LV_COLOR_MAKE(50, 50, 50);
    switch_bg_style.body.grad_color = LV_COLOR_MAKE(50, 50, 50);
    switch_bg_style.body.border.color = LV_COLOR_MAKE(50, 50, 50);
    switch_bg_style.body.radius = 8;
    switch_bg_style.body.border.width = 10; 

    lv_style_copy(&switch_indic_style, &lv_style_plain);
    switch_indic_style.body.main_color = LV_COLOR_MAKE(0, 255, 0);
    switch_indic_style.body.grad_color = LV_COLOR_MAKE(0, 255, 0);
    switch_indic_style.body.border.color = LV_COLOR_MAKE(0, 255, 0);
    switch_indic_style.body.radius = 8;
    switch_indic_style.body.border.width = 8;

    // Create Objects //
    backround = createBtn(lv_scr_act(), 0, 0, SCREEN_WIDTH_MAX, SCREEN_HEIGHT_MAX,
            &plain_black, &plain_black, LV_BTN_ACTION_LONG_PR_REPEAT, NULL, 0, "");

    illini_label = createLabel(lv_scr_act(), 0, 0, 0, 0, &label_style, "ILL-INI");
    lv_obj_align(illini_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    robot_label = createLabel(lv_scr_act(), 0, 0, 0, 0, &label_style, "comp 18");
    lv_obj_align(robot_label, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

    auton_select_label = createLabel(lv_scr_act(), 30, 35, 0, 0, &label_style, "Choose Auton:");

    comp_auton_btn = createBtn(lv_scr_act(), 45, 65, 100, 35, 
        &comp_auton_style_sel, &comp_auton_style, LV_BTN_ACTION_CLICK, autonSelection, 1, "WinPoint");
    lv_btn_set_state(comp_auton_btn, LV_BTN_STATE_REL);

    skills_auton_btn = createBtn(lv_scr_act(), 45, 65+35+10, 100, 35, 
        &skills_auton_style_sel, &skills_auton_style, LV_BTN_ACTION_CLICK, autonSelection, 2, "Elims");
    lv_btn_set_state(skills_auton_btn, LV_BTN_STATE_REL);

    // no_auton_btn = createBtn(lv_scr_act(), 45, 110+35+10, 100, 35,
    //     &no_auton_style_sel, &no_auton_style, LV_BTN_ACTION_CLICK, autonSelection, 3, "None");
    // lv_btn_set_state(no_auton_btn, LV_BTN_STATE_REL);

    if (gui::selected_auton != gui::autonomous_type::NO_SELECTION)
    {
        switch(gui::selected_auton)
        {
            case(gui::autonomous_type::AUTON_COMP):
                lv_btn_set_state(comp_auton_btn, LV_BTN_STATE_PR);
                break;
            case(gui::autonomous_type::AUTON_SKILLS):
                lv_btn_set_state(skills_auton_btn, LV_BTN_STATE_PR);
                break;
            case(gui::autonomous_type::AUTON_NONE):
                lv_btn_set_state(no_auton_btn, LV_BTN_STATE_PR);
                break;
        }
    }

    tank_drive_btn = createBtn(lv_scr_act(), 180, 40, 70, 35,
        &modeBtnStyle, &modeBtnStyle, LV_BTN_ACTION_CLICK, NULL, 0, "Tank");
    tank_drive_switch = lv_sw_create(lv_scr_act(), NULL);
    lv_obj_set_pos(tank_drive_switch, 180+70+10, 40-2);
    lv_obj_set_size(tank_drive_switch, 70, 40);
    lv_sw_set_style(tank_drive_switch, LV_SW_STYLE_KNOB_OFF, &switch_knob_off_style);
    lv_sw_set_style(tank_drive_switch, LV_SW_STYLE_KNOB_ON, &switch_knob_on_style);
    lv_sw_set_style(tank_drive_switch, LV_SW_STYLE_BG, &switch_bg_style);
    lv_sw_set_style(tank_drive_switch, LV_SW_STYLE_INDIC, &switch_indic_style);
    lv_sw_off(tank_drive_switch);
    lv_obj_set_free_num(tank_drive_switch, 1);
    lv_sw_set_anim_time(tank_drive_switch, 300);
    lv_sw_set_action(tank_drive_switch, driveTypeUpdate);

    arcade_drive_btn = createBtn(lv_scr_act(), 180, 40+35+10, 70, 35,
        &modeBtnStyle, &modeBtnStyle, LV_BTN_ACTION_CLICK, NULL, 0, "Arcade");
    arcade_drive_switch = lv_sw_create(lv_scr_act(), NULL);
    lv_obj_set_pos(arcade_drive_switch, 180+70+10, 40+45-2);
    lv_obj_set_size(arcade_drive_switch, 70, 40);
    lv_sw_set_style(arcade_drive_switch, LV_SW_STYLE_KNOB_OFF, &switch_knob_off_style);
    lv_sw_set_style(arcade_drive_switch, LV_SW_STYLE_KNOB_ON, &switch_knob_on_style);
    lv_sw_set_style(arcade_drive_switch, LV_SW_STYLE_BG, &switch_bg_style);
    lv_sw_set_style(arcade_drive_switch, LV_SW_STYLE_INDIC, &switch_indic_style);
    lv_sw_on(arcade_drive_switch);
    lv_obj_set_free_num(arcade_drive_switch, 2);
    lv_sw_set_anim_time(tank_drive_switch, 300);
    lv_sw_set_action(arcade_drive_switch, driveTypeUpdate);

    manual_trigger_btn1 = createBtn(lv_scr_act(), 0, 0, 130, 35,
        &manualPressed, &manualReleased, LV_BTN_ACTION_CLICK, manualTrigger, 1, "LLEMU Open");
    lv_obj_align(manual_trigger_btn1, NULL, LV_ALIGN_IN_RIGHT_MID, -10, -35);
    
    manual_trigger_btn2 = createBtn(lv_scr_act(), 0, 0, 130, 35,
        &manualPressed, &manualReleased, LV_BTN_ACTION_CLICK, manualTrigger, 2, "Wings Close");
    lv_obj_align(manual_trigger_btn2, NULL, LV_ALIGN_IN_RIGHT_MID, -10, 35);

    raze_ss = new pros::Task([=] { raze_ss_runner(); });
    raze_ss->set_priority(TASK_PRIORITY_LOW);
}
