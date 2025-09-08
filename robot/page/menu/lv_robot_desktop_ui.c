/**
 * @file lv_robot_desktop_ui.c
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
#include "lv_robot_desktop_ui.h"

/*********************
 *      DEFINES
 *********************/
#define LV_VIDEO_STANDBY_1 "../../assert/video/通用-早上-刚睡醒打哈欠.1.mp4"
#define LV_VIDEO_STANDBY_2 "../../assert/video/通用-早上-好奇.1.mp4"
#define LV_VIDEO_STANDBY_3 "../../assert/video/通用-早上-平静.1.mp4"
#define LV_VIDEO_STANDBY_4 "../../assert/video/通用-早上-期盼.1.mp4"
#define LV_VIDEO_STANDBY_5 "../../assert/video/通用-晚上-打呼噜.1.mp4"
#define LV_VIDEO_STANDBY_6 "../../assert/video/通用-晚上-梦游.1.mp4"
#define LV_VIDEO_STANDBY_7 "../../assert/video/通用-晚上-睡觉.1.mp4"
#define LV_VIDEO_STANDBY_8 "../../assert/video/高冷-陌生人发出指令-耍酷.1.mp4"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void robot_style_init();
static void robot_show_desktop_page(void);
static void robot_exit_desktop_page(void);
static void robot_desktop_page_create(void);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t style;
static lv_obj_t *desktop_page;

static lv_widget_t lv_page_desktop = {
    .page = NULL,
    .name = "待机桌面",
    .type = LV_PAGE_DESKTOP,
    .path = NULL,
    .create_func = robot_desktop_page_create,
    .show_func = robot_show_desktop_page,
    .exit_func = robot_exit_desktop_page,
    .reserved = NULL
};
static const char *video_paths[] = {
    LV_VIDEO_STANDBY_1,
    LV_VIDEO_STANDBY_2,
    LV_VIDEO_STANDBY_3,
    LV_VIDEO_STANDBY_4,
    LV_VIDEO_STANDBY_5,
    LV_VIDEO_STANDBY_6,
    LV_VIDEO_STANDBY_7,
    LV_VIDEO_STANDBY_8
};


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_robot_desktop_ui_register()
{
    lv_page_manager_add(&lv_page_desktop);
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

static void robot_show_desktop_page(void)
{
    uint32_t seed = (uint32_t)lv_tick_get();
    lv_rand_set_seed(seed);
    uint32_t idx = lv_rand(1, 8);

    //随机选择
    lv_obj_t *player = lv_ffmpeg_player_create(desktop_page);
    lv_ffmpeg_player_set_src(player, video_paths[idx -1]);
    lv_ffmpeg_player_set_auto_restart(player, true);
    lv_ffmpeg_player_set_cmd(player, LV_FFMPEG_PLAYER_CMD_START);
    lv_obj_center(player);

    return;
}

static void robot_exit_desktop_page(void)
{
    //1.销毁或者暂停定时器
}

static void robot_desktop_page_create(void)
{
    robot_style_init();

    desktop_page = lv_obj_create(NULL);
    lv_obj_add_style(desktop_page, &style, 0);
    lv_obj_center(desktop_page);

    lv_page_desktop.page = desktop_page;
    lv_page_manager_add(&lv_page_desktop);

    return;
}