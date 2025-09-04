/**
 * @file lv_robot_record_ui.c
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
#include "lv_robot_record_ui.h"

/*********************
 *      DEFINES
 *********************/
#define RECORD_TIME 300 //5min

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void robot_style_init();
static void robot_show_record_page(void);
static void robot_exit_record_page(void);
static void robot_record_page_create(void);
static void record_event_hander(lv_event_t *);
static void consecutive_click_event_hander(lv_event_t *);
static void record_btn_event_handler(lv_event_t *);
static void timer_callback_1(lv_timer_t *);
static void timer_callback_2(lv_timer_t *);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t style;
static lv_style_t area_style;
static lv_style_t indicator_style;
static lv_style_t main_style;
static lv_style_t btn_style;

static lv_obj_t *bg_image;
static lv_obj_t *tips_cont;
static lv_obj_t *indicator;
static lv_obj_t *record_page;
static lv_obj_t *time_area;
static lv_timer_t *timer_1;
static lv_timer_t *timer_2;//录像的秒数

static uint32_t tick_sec = 3;
static uint32_t record_sec = 0;
static bool btn_control_flag = false;
static bool click_invalid = false;
static uint32_t EV_RECORD_START;//自定义事件

static lv_widget_t lv_page_record = {
    .page = NULL,
    .name = "录像",
    .type = LV_PAGE_RECORD,
    .path = "V:icon/img_app_icon_record.png",
    .create_func = robot_record_page_create,
    .show_func = robot_show_record_page,
    .exit_func = robot_exit_record_page,
    .reserved = NULL
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_robot_record_ui_register()
{
    lv_page_manager_add(&lv_page_record);
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

    //area_style
    static lv_grad_dsc_t grad;
    grad.dir = LV_GRAD_DIR_VER;
    grad.stops_count = 2;
    grad.stops[0].color = lv_color_hex(0x000000);
    grad.stops[0].opa = LV_OPA_COVER;
    grad.stops[1].color = lv_color_hex(0x000000);
    grad.stops[1].opa = LV_OPA_TRANSP;
    grad.stops[0].frac = 0;
    grad.stops[1].frac = 255;
    lv_style_init(&area_style);
    lv_style_copy(&area_style, &style);
    lv_style_set_bg_grad(&area_style, &grad);

    //indicator_style
    lv_style_init(&indicator_style);
    lv_style_set_arc_color(&indicator_style, lv_color_hex(0x56D6AA));
    lv_style_set_arc_opa(&indicator_style, LV_OPA_60);
    lv_style_set_arc_width(&indicator_style, 6);

    //main_style
    lv_style_init(&main_style);
    lv_style_set_arc_color(&main_style, lv_color_hex(0xEBEBF5));
    lv_style_set_arc_opa(&main_style, LV_OPA_60);
    lv_style_set_arc_width(&main_style, 6);

    //btn_style
    static lv_grad_dsc_t grad_btn;
    grad_btn.dir = LV_GRAD_DIR_LINEAR;
    grad_btn.stops_count = 2;
    grad_btn.stops[0].color = lv_color_hex(0xB0F99C);
    grad_btn.stops[0].opa = LV_OPA_COVER;
    grad_btn.stops[0].frac = 0;
    grad_btn.stops[1].color = lv_color_hex(0x3ACBAF);
    grad_btn.stops[1].opa = LV_OPA_COVER;
    grad_btn.stops[1].frac = 255;
    lv_point_t start = {0, 0};
    lv_point_t end = {LV_PCT(100), LV_PCT(100)};
    grad_btn.params.linear.start = start;
    grad_btn.params.linear.end = end;
    lv_style_init(&btn_style);
    lv_style_set_bg_grad(&btn_style, &grad_btn);

    return;
}

static void robot_show_record_page(void)
{
    bg_image = lv_image_create(record_page);
    lv_obj_add_style(bg_image, &style, 0);
    lv_obj_set_size(bg_image, lv_pct(100), lv_pct(100));
    lv_image_set_src(bg_image, "V:png/img_camera_backup.png");
    lv_img_set_zoom(bg_image, 128);
    lv_obj_center(bg_image);

    tips_cont = lv_obj_create(record_page);
    lv_obj_set_size(tips_cont, 452, 80);
    lv_obj_add_style(tips_cont, &style, 0);
    lv_obj_set_style_bg_color(tips_cont, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(tips_cont, LV_OPA_20, LV_PART_MAIN);
    lv_obj_set_style_radius(tips_cont, 30, LV_PART_MAIN);
    lv_obj_align(tips_cont, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t *label = lv_label_create(tips_cont);
    lv_label_set_text(label, "拍3下我的脑袋就可以录像了~");
    lv_obj_set_style_text_opa(label, LV_OPA_80, LV_PART_MAIN);
    lv_obj_set_style_text_font(label, fzlth_font_30B, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_event_cb(record_page, record_event_hander, EV_RECORD_START, NULL);
    lv_obj_add_event_cb(record_page, consecutive_click_event_hander, LV_EVENT_ALL, NULL);

    return;
}

static void robot_exit_record_page(void)
{
    tick_sec = 3;
    record_sec = 0;
    btn_control_flag = false;
    click_invalid = false;

    //1.销毁或者暂停定时器
    if (NULL != timer_2)
    {
        lv_timer_del(timer_2);
        timer_2 = NULL;
    }

    if (NULL != timer_1)
    {
        lv_timer_del(timer_1);
        timer_1 = NULL;
    }
}

static void robot_record_page_create(void)
{
    robot_style_init();

    record_page = lv_obj_create(NULL);
    lv_obj_add_style(record_page, &style, 0);
    lv_obj_center(record_page);

    lv_page_record.page = record_page;
    lv_page_manager_add(&lv_page_record);

    //注册自定义事件
    EV_RECORD_START = lv_event_register_id();

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

        lv_obj_t *label_num = lv_obj_create(record_page);
        lv_obj_remove_style_all(label_num);
        lv_obj_set_size(label_num, 60, 140);
        lv_obj_set_style_bg_opa(label_num, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_align(label_num, LV_ALIGN_CENTER, 0, 0);

        lv_obj_t *label1 = lv_label_create(label_num);
        lv_obj_set_style_text_opa(label1, LV_OPA_TRANSP, LV_PART_MAIN);

        // 创建定时器，执行3次后自动删除
        timer_1 = lv_timer_create(timer_callback_1, 1000, label1);
        lv_timer_set_repeat_count(timer_1, 4);
        lv_timer_set_auto_delete(timer_1, false);//退出时手动删除定时器        
    }

    return;
}

static void timer_callback_1(lv_timer_t *timer)
{
    LV_LOG_USER("录像倒计时: %ds", tick_sec);

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
        lv_obj_clean(record_page);
        lv_obj_send_event(record_page, EV_RECORD_START, NULL);
        //TODO: 通知开始录像

    }
    tick_sec--;

    return;
}

static void record_event_hander(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (EV_RECORD_START == code)
    {
        //顶部渐变区域
        lv_obj_t *indicator_area = lv_obj_create(record_page);
        lv_obj_set_size(indicator_area, 502, 75);
        lv_obj_align(indicator_area, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_add_style(indicator_area, &area_style, 0);

        lv_obj_t *led = lv_obj_create(indicator_area);
        lv_obj_remove_style_all(led);
        lv_obj_set_style_bg_opa(led, LV_OPA_COVER, 0);
        lv_obj_set_style_radius(led, LV_RADIUS_CIRCLE, LV_PART_MAIN);
        lv_obj_set_style_bg_color(led, lv_color_hex(0xFF5C5C), LV_PART_MAIN);
        lv_obj_align(led, LV_ALIGN_TOP_LEFT, 203, 32);
        lv_obj_set_size(led, 12, 12);

        time_area = lv_obj_create(indicator_area);
        lv_obj_remove_style_all(time_area);
        lv_obj_set_size(time_area, 85, 56);//原75->85，字体不同
        lv_obj_set_style_bg_opa(time_area, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_align(time_area, LV_ALIGN_TOP_LEFT, 226, 10);

        lv_obj_t *label = lv_label_create(time_area);
        lv_label_set_text(label, "00:00");
        lv_obj_set_style_text_font(label, fzlth_font_30, LV_PART_MAIN);
        lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        lv_obj_set_style_text_opa(label, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_center(label);

        //底部圆弧区域
        lv_obj_t *arc = lv_arc_create(record_page);
        lv_obj_set_size(arc, 110, 110);
        lv_arc_set_range(arc, 0, RECORD_TIME);
        lv_arc_set_rotation(arc, 270);
        lv_arc_set_bg_angles(arc, 0, 360);
        lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
        lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_style(arc, &indicator_style, LV_PART_INDICATOR);
        lv_obj_add_style(arc, &main_style, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(arc, LV_OPA_TRANSP, 0);
        lv_obj_align(arc, LV_ALIGN_BOTTOM_MID, 0, -20);
        timer_2 = lv_timer_create(timer_callback_2, 1000, arc);

        lv_obj_t *btn = lv_btn_create(record_page);
        lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
        lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);
        lv_obj_set_size(btn, 34, 34);
        lv_obj_add_style(btn, &btn_style, LV_PART_MAIN);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -58);
        lv_obj_add_event_cb(btn, record_btn_event_handler, LV_EVENT_CLICKED, NULL);
    }

    return;
}

static void timer_callback_2(lv_timer_t *timer)
{
    lv_obj_t *label = lv_obj_get_child(time_area, 0);
    lv_obj_t *arc = (lv_obj_t *)lv_timer_get_user_data(timer);

    lv_label_set_text_fmt(label, "%02d:%02d", record_sec / 60, record_sec % 60);
    lv_arc_set_value(arc, record_sec);

    record_sec = record_sec++ >= RECORD_TIME? 0 : record_sec;
}

static void record_btn_event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (LV_EVENT_CLICKED == code)
    {
        //反转开关标志
        btn_control_flag = !btn_control_flag;
        if (true == btn_control_flag)
        {
            //暂停定时器计时
            lv_timer_pause(timer_2);
            //TODO: 通知停止录像

        }
        if (false == btn_control_flag)
        {
            //恢复定时器计时
            lv_timer_resume(timer_2);
            record_sec = 0;
            //TODO: 通知重新录像

        }
    }

    return;
}