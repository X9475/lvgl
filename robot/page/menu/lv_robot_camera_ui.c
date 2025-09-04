/**
 * @file lv_robot_camera_ui.c
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
#include "lv_robot_camera_ui.h"

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
static void robot_show_camera_page(void);
static void robot_exit_camera_page(void);
static void robot_camera_page_create(void);
static void consecutive_click_event_hander(lv_event_t *);
static void timer_callback(lv_timer_t *);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t style;
static lv_obj_t *bg_image;
static lv_obj_t *tips_cont;
static lv_obj_t *camera_page;
static lv_timer_t *timer_label;

static uint32_t tick_sec = 3;
static bool click_invalid = false;

static lv_widget_t lv_page_camera = {
    .page = NULL,
    .name = "拍照",
    .type = LV_PAGE_CAMERA,
    .path = "V:icon/img_app_icon_camera.png",
    .create_func = robot_camera_page_create,
    .show_func = robot_show_camera_page,
    .exit_func = robot_exit_camera_page,
    .reserved = NULL
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_robot_camera_ui_register()
{
    lv_page_manager_add(&lv_page_camera);
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

static void robot_show_camera_page(void)
{
    bg_image = lv_image_create(camera_page);
    lv_obj_add_style(bg_image, &style, 0);
    lv_obj_set_size(bg_image, lv_pct(100), lv_pct(100));
    lv_image_set_src(bg_image, "V:png/img_camera_backup.png");
    lv_img_set_zoom(bg_image, 128);
    lv_obj_center(bg_image);

    tips_cont = lv_obj_create(camera_page);
    lv_obj_set_size(tips_cont, 452, 80);
    lv_obj_add_style(tips_cont, &style, 0);
    lv_obj_set_style_bg_color(tips_cont, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(tips_cont, LV_OPA_20, LV_PART_MAIN);
    lv_obj_set_style_radius(tips_cont, 30, LV_PART_MAIN);
    lv_obj_align(tips_cont, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t *label = lv_label_create(tips_cont);
    lv_label_set_text(label, "拍3下我的脑袋就可以拍照了~");
    lv_obj_set_style_text_opa(label, LV_OPA_80, LV_PART_MAIN);
    lv_obj_set_style_text_font(label, fzlth_font_30B, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    //触碰屏幕三下模拟拍打
    lv_obj_add_event_cb(camera_page, consecutive_click_event_hander, LV_EVENT_ALL, NULL);

    return;
}

static void robot_exit_camera_page(void)
{
    tick_sec = 3;
    click_invalid = false;

    //1.销毁或者暂停定时器
    if (NULL != timer_label)
    {
        lv_timer_del(timer_label);
    }
}

static void robot_camera_page_create(void)
{
    robot_style_init();

    camera_page = lv_obj_create(NULL);
    lv_obj_add_style(camera_page, &style, 0);
    lv_obj_center(camera_page);

    lv_page_camera.page = camera_page;
    lv_page_manager_add(&lv_page_camera);

    return;
}

static void consecutive_click_event_hander(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    //连续点击3次
    if (LV_EVENT_TRIPLE_CLICKED == code && !click_invalid)
    {
        click_invalid = true;

        //删除小贴士
        lv_obj_del(tips_cont);

        lv_obj_t *label_num = lv_obj_create(camera_page);
        lv_obj_remove_style_all(label_num);
        lv_obj_set_size(label_num, 60, 140);
        lv_obj_set_style_bg_opa(label_num, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_align(label_num, LV_ALIGN_CENTER, 0, 0);
        lv_obj_t *label = lv_label_create(label_num);
        lv_obj_set_style_text_opa(label, LV_OPA_TRANSP, LV_PART_MAIN);

        // 创建定时器，执行3次后自动删除
        timer_label = lv_timer_create(timer_callback, 1000, label);
        lv_timer_set_repeat_count(timer_label, 4);
        lv_timer_set_auto_delete(timer_label, false);//退出时手动删除定时器
    }
}

static void timer_callback(lv_timer_t *timer)
{
    LV_LOG_USER("拍照倒计时: %ds", tick_sec);

    lv_obj_t *label = lv_timer_get_user_data(timer);
    lv_label_set_text_fmt(label, "%d", tick_sec);
    lv_obj_set_style_text_font(label, fzlth_font_100B, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_opa(label, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_center(label);

    if (tick_sec == 0)
    {
        tick_sec = 3;
        lv_obj_clean(camera_page);
        //TODO: 通知发送相机抓拍事件

    }
    tick_sec--;

    return;
}