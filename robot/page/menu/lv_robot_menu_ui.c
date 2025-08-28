/**
 * @file lv_robot_menu_ui.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_robot_menu_ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void robot_style_init();
static void robot_show_menu_page(void);
static void robot_exit_menu_page(void);
static void robot_menu_page_create(void);
static void robot_menu_item_event_handler(lv_event_t *);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t style;
static lv_obj_t *menu_page;

static lv_widget_t lv_page_menu = {
    .page = NULL,
    .name = "菜单",
    .type = LV_PAGE_MENU,
    .image = NULL,
    .create_func = robot_menu_page_create,
    .show_func = robot_show_menu_page,
    .exit_func = robot_exit_menu_page,
    .reserved = NULL
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_robot_menu_ui_register()
{
    lv_page_manager_add(&lv_page_menu);
}

/**********************
 *  STATIC FUNCTIONS
 **********************/
static void robot_style_init()
{
    lv_style_init(&style);
    lv_style_set_pad_all(&style, 0);
    lv_style_set_border_width(&style, 0);
    lv_style_set_radius(&style, 0);
}

static void robot_show_menu_page(void)
{
    lv_obj_t *cont = lv_obj_create(menu_page);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_add_style(cont, &style, 0);
    lv_obj_center(cont);

    lv_obj_t *imgbtn = NULL;
    lv_obj_t *label = NULL;
    for (uint8_t idx = 0; idx < 9; idx++)
    {
        void *page_ptr = lv_page_foreach_find(idx);
        if (NULL != page_ptr)
        {
            lv_widget_t *widget = (lv_widget_t *)page_ptr;
            lv_obj_t *obj = lv_obj_create(cont);
            lv_obj_set_size(obj, 350, 380);
            lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_add_style(obj, &style, 0);

            // 添加imgbtn
            imgbtn = lv_imgbtn_create(obj);
            lv_imgbtn_set_src(imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, widget->image, NULL);
            lv_imgbtn_set_src(imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, widget->image, NULL);
            lv_obj_set_size(imgbtn, widget->image->header.w, widget->image->header.h);
            lv_obj_set_user_data(imgbtn, widget);
            lv_obj_center(imgbtn);

            label = lv_label_create(obj);
            lv_label_set_text(label, widget->name);
            lv_obj_set_style_text_font(label, fzlth_font_40, 0);
            lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
            lv_obj_align_to(label, imgbtn, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

            //注册事件处理回调
            lv_obj_add_event_cb(imgbtn, robot_menu_item_event_handler, LV_EVENT_CLICKED, widget);
        }
    }

    //记忆上次从菜单进入的页面位置
    lv_obj_t *cur_screen = lv_scr_act();
    lv_widget_t *widget = lv_find_widget_from_page(cur_screen);
    int32_t index = widget->type > LV_PAGE_RELAX ? 0 : widget->type;
    lv_obj_scroll_to_view(lv_obj_get_child(cont, index), LV_ANIM_OFF);

    return;
}

static void robot_exit_menu_page(void)
{
    //1.销毁或者暂停定时器
}

static void robot_menu_page_create(void)
{
    robot_style_init();

    menu_page = lv_obj_create(NULL);
    lv_obj_add_style(menu_page, &style, 0);
    lv_obj_center(menu_page);

    //防止从对象传递手势事件给父级
    // lv_obj_remove_flag(menu_page, LV_OBJ_FLAG_GESTURE_BUBBLE);
    // lv_obj_add_flag(menu_page, LV_OBJ_FLAG_CLICKABLE);
    lv_page_menu.page = menu_page;
    lv_page_manager_add(&lv_page_menu);

    return;
}

static void robot_menu_item_event_handler(lv_event_t *e)
{
    lv_obj_t *image = lv_event_get_target(e);
    lv_widget_t *data = lv_obj_get_user_data(image);
    lv_event_code_t code = lv_event_get_code(e);

    if (LV_EVENT_CLICKED == code)
    {
        //目标页面跳转，页面入栈
        lv_widget_t *cur_screen = lv_page_manager_change(data->type);
        //页面回收前的操作
        if (NULL != cur_screen->exit_func)
        {
            cur_screen->exit_func();
        }
        //清除页面内容
        lv_obj_clean(lv_page_menu.page);
        //激活屏幕
        lv_scr_load_anim(cur_screen->page, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    }
}