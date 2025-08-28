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
LV_IMG_DECLARE(img_desktop_backup);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t style;
static lv_obj_t *desktop_page;

static lv_widget_t lv_page_desktop = {
    .page = NULL,
    .name = "待机桌面",
    .type = LV_PAGE_DESKTOP,
    .image = &img_desktop_backup,
    .create_func = robot_desktop_page_create,
    .show_func = robot_show_desktop_page,
    .exit_func = robot_exit_desktop_page,
    .reserved = NULL
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
    lv_obj_t *img = lv_img_create(desktop_page);
    lv_obj_set_size(img, LV_PCT(100), LV_PCT(100));
    lv_image_set_src(img, &img_desktop_backup);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_image_set_scale(img, 128);

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