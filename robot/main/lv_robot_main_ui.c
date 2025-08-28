/**
 * @file lv_robot_main_ui.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/*********************
 *     INCLUDES
 *********************/
#include "lv_robot_main_ui.h"

/*********************
 *     DEFINES
 *********************/
lv_obj_t *buttom_line;

/**********************
 *     TYPEDEFS
 **********************/

/**********************
 * STATIC VARIABLES
 **********************/

/**********************
 * STATIC PROTOTYPES
 **********************/
static void lv_robot_page_register();

/**********************
 * GLOBAL FUNCTIONS
 **********************/
void lv_robot_main()
{
    //注册页面加入管理链表
    lv_robot_page_register();

    //创建屏幕对象
    for (uint8_t idx = 0; idx < LV_ROBOT_MENU_ITEM_COUNT; idx++)
    {
        void *page_ptr = lv_page_foreach_find(idx);
        if (NULL != page_ptr) {
            lv_widget_t *widget = (lv_widget_t *)page_ptr;
            widget->create_func();
        }
    }

    /*底部APP返回横条*/
    buttom_line = lv_obj_create(lv_layer_top());
    lv_obj_set_size(buttom_line, 80, 6);
    lv_obj_set_style_radius(buttom_line, 3, 0);
    lv_obj_set_style_bg_color(buttom_line, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_align(buttom_line, LV_ALIGN_BOTTOM_MID, 0, -17); //距离底部向上17px
    lv_obj_set_style_opa(buttom_line, LV_OPA_COVER, 0);
    lv_obj_add_flag(buttom_line, LV_OBJ_FLAG_HIDDEN);

    /*为每个屏幕对象添加手势滑动*/
    for (uint8_t idx = 0; idx < LV_ROBOT_MENU_ITEM_COUNT; idx++)
    {
        void *page_ptr = lv_page_foreach_find(idx);
        if (NULL != page_ptr) {
            lv_widget_t *widget = (lv_widget_t *)page_ptr;
            lv_obj_set_user_data(widget->page, widget);
            lv_obj_add_event_cb(widget->page, lv_page_manager_gesture_event, LV_EVENT_GESTURE, NULL);
        }
    }

    //加载桌面显示
    lv_widget_t *screen = lv_page_manager_change(LV_PAGE_DESKTOP);
    lv_scr_load_anim(screen->page, LV_SCR_LOAD_ANIM_NONE, 0, 0, false);

    return;
}

/**********************
 * STATIC FUNCTIONS
 **********************/
static void lv_robot_page_register()
{
    //菜单页面加入链表
    lv_robot_menu_ui_register();
    //桌面页面加入链表
    lv_robot_desktop_ui_register();
    //设置页面加入链表
    lv_robot_setting_ui_register();
    //好运日签加入链表
    lv_robot_luckcard_ui_register();
    //解惑魔盒加入链表
    lv_robot_mystery_ui_register();
    //天气加入链表
    lv_robot_weather_ui_register();
    //相机加入链表
    lv_robot_camera_ui_register();
    //录像加入链表
    lv_robot_record_ui_register();
    //叮咚策略器加入链表
    lv_robot_strategy_ui_register();
    //待机样式加入链表
    lv_robot_standstyle_ui_register();
    //解压加入链表
    lv_robot_relax_ui_register();

    return;
}
