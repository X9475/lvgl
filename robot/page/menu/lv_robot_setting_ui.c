/**
 * @file lv_robot_setting_ui.c
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
#include "lv_robot_setting_ui.h"

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
static void robot_show_setting_page(void);
static void robot_exit_setting_page(void);
static void robot_setting_page_create(void);
static void slider_pressed_event_cb(lv_event_t *);
static void slider_released_event_cb(lv_event_t *);
static void slider_value_change_event_cb(lv_event_t *);

/**********************
 * GLOBAL PROTOTYPES
 **********************/
LV_IMG_DECLARE(ezviz_setting_light);
LV_IMG_DECLARE(ezviz_setting_volume);
LV_IMG_DECLARE(img_app_icon_settings);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t style;
static lv_style_t style_label;
static lv_style_t style_main;
static lv_style_t style_knob;
static lv_style_t style_indicator_light;
static lv_style_t style_indicator_volume;
static lv_obj_t *slider_light;
static lv_obj_t *slider_volume;
static lv_obj_t *setting_page;

static uint32_t last_light_value = 30;//亮度的值
static uint32_t last_volume_value = 60;//音量的值

static lv_widget_t lv_page_setting = {
    .page = NULL,
    .name = "设置",
    .type = LV_PAGE_SETTING,
    .image = &img_app_icon_settings,
    .create_func = robot_setting_page_create,
    .show_func = robot_show_setting_page,
    .exit_func = robot_exit_setting_page,
    .reserved = NULL
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_robot_setting_ui_register()
{
    lv_page_manager_add(&lv_page_setting);
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

    //style_label
    lv_style_init(&style_label);
    lv_style_set_radius(&style_label, LV_RADIUS_CIRCLE);
    lv_style_set_bg_opa(&style_label, LV_OPA_20);
    lv_style_set_text_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_align(&style_label, LV_TEXT_ALIGN_CENTER);
    lv_style_set_bg_color(&style_label, lv_color_hex(0x000000));

    //style_main
    lv_style_init(&style_main);
    lv_style_set_radius(&style_main, 30);
    lv_style_set_pad_all(&style_main, 0);
    lv_style_set_bg_opa(&style_main, LV_OPA_10);
    lv_style_set_bg_color(&style_main, lv_color_hex(0xFFFFFF));

    //style_knob
    lv_style_init(&style_knob);
    lv_style_set_pad_all(&style_knob, 0);
    lv_style_set_bg_opa(&style_knob, LV_OPA_TRANSP);

    //style_indicator_light
    lv_style_init(&style_indicator_light);
    lv_style_set_pad_all(&style_indicator_light, 0);
    lv_style_set_radius(&style_indicator_light, 0);
    lv_style_set_bg_opa(&style_indicator_light, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator_light, lv_color_hex(0xAFF99C));

    //style_indicator_volume
    lv_style_init(&style_indicator_volume);
    lv_style_set_pad_all(&style_indicator_volume, 0);
    lv_style_set_radius(&style_indicator_volume, 0);
    lv_style_set_bg_opa(&style_indicator_volume, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator_volume, lv_color_hex(0x6478FF));
}

static void robot_show_setting_page(void)
{
    lv_obj_t *cont = lv_obj_create(setting_page);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_add_style(cont, &style, 0);
    lv_obj_center(cont);

    //亮度设置
    slider_light = lv_slider_create(cont);
    lv_obj_add_style(slider_light, &style_main, LV_PART_MAIN);
    lv_obj_add_style(slider_light, &style_knob, LV_PART_KNOB);
    lv_obj_add_style(slider_light, &style_indicator_light, LV_PART_INDICATOR);
    lv_slider_set_mode(slider_light, LV_SLIDER_MODE_NORMAL);
    lv_obj_set_size(slider_light, 432, 128);
    lv_slider_set_range(slider_light, 1, 100);
    lv_obj_align(slider_light, LV_ALIGN_TOP_LEFT, 35, 50);
    lv_slider_set_value(slider_light, last_light_value, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_light, slider_pressed_event_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(slider_light, slider_released_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(slider_light, slider_value_change_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *label1 = lv_label_create(slider_light);
    lv_obj_set_size(label1, 88, 88);
    lv_obj_add_style(label1, &style_label, LV_PART_MAIN);
    lv_obj_align(label1, LV_ALIGN_LEFT_MID, 20, 0);
    lv_obj_t *img1 = lv_img_create(label1);
    lv_img_set_src(img1, &ezviz_setting_light);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);

    //音量设置
    slider_volume = lv_slider_create(cont);
    lv_obj_add_style(slider_volume, &style_main, LV_PART_MAIN);
    lv_obj_add_style(slider_volume, &style_knob, LV_PART_KNOB);
    lv_obj_add_style(slider_volume, &style_indicator_volume, LV_PART_INDICATOR);
    lv_slider_set_mode(slider_volume, LV_SLIDER_MODE_NORMAL);
    lv_obj_set_size(slider_volume, 432, 128);
    lv_slider_set_range(slider_volume, 1, 100);
    lv_obj_align(slider_volume, LV_ALIGN_TOP_LEFT, 35, 208);
    lv_slider_set_value(slider_volume, last_volume_value, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_volume, slider_pressed_event_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(slider_volume, slider_released_event_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(slider_volume, slider_value_change_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *label2 = lv_label_create(slider_volume);
    lv_obj_set_size(label2, 88, 88);
    lv_obj_add_style(label2, &style_label, LV_PART_MAIN);
    lv_obj_align(label2, LV_ALIGN_LEFT_MID, 20, 0);
    lv_obj_t *img2 = lv_img_create(label2);
    lv_img_set_src(img2, &ezviz_setting_volume);
    lv_obj_align(img2, LV_ALIGN_CENTER, 0, 0);

    //显示底部横条
    lv_obj_clear_flag(buttom_line, LV_OBJ_FLAG_HIDDEN);

    return;
}

static void robot_exit_setting_page(void)
{
    //1.销毁或者暂停定时器
}

static void robot_setting_page_create(void)
{
    robot_style_init();

    setting_page = lv_obj_create(NULL);
    lv_obj_add_style(setting_page, &style, 0);
    lv_obj_center(setting_page);

    lv_page_setting.page = setting_page;
    lv_page_manager_add(&lv_page_setting);

    return;
}

static void slider_pressed_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);

    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
}

static void slider_released_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);

    lv_obj_add_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
}

static void slider_value_change_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);

    uint32_t value = lv_slider_get_value(obj);

    if (obj == slider_light)
    {
        LV_LOG_USER("light value:%d", value);
        last_light_value = value;
        //TODO: 发送亮度调节的事件

    }
    else if (obj == slider_volume)
    {
        LV_LOG_USER("volume value:%d", value);
        last_volume_value = value;
        //TODO: 发送音量调节的事件

    }

    return;
}