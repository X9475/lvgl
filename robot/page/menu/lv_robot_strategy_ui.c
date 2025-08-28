/**
 * @file lv_robot_strategy_ui.c
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
#include "lv_robot_strategy_ui.h"

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
static void robot_show_strategy_page(void);
static void robot_exit_strategy_page(void);
static void robot_strategy_page_create(void);

/**********************
 * GLOBAL PROTOTYPES
 **********************/
LV_IMG_DECLARE(img_app_icon_strategy);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t style;
static lv_obj_t * strategy_page;

static lv_widget_t lv_page_strategy = {
    .page = NULL,
    .name = "叮咚策略器",
    .type = LV_PAGE_STRATEGY,
    .image = &img_app_icon_strategy,
    .create_func = robot_strategy_page_create,
    .show_func = robot_show_strategy_page,
    .exit_func = robot_exit_strategy_page,
    .reserved = NULL
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_robot_strategy_ui_register()
{
    lv_page_manager_add(&lv_page_strategy);
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

static void robot_show_strategy_page(void)
{
    lv_obj_t *cont = lv_obj_create(strategy_page);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_add_style(cont, &style, 0);
    lv_obj_center(cont);

    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_center(btn);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(label, lv_page_strategy.name);
    lv_obj_set_style_text_font(label, fzlth_font_30, 0);
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(label);

    return;
}

static void robot_exit_strategy_page(void)
{
    //1.销毁或者暂停定时器   
}

static void robot_strategy_page_create(void)
{
    robot_style_init();

    strategy_page = lv_obj_create(NULL);
    lv_obj_add_style(strategy_page, &style, 0);
    lv_obj_center(strategy_page);

    lv_page_strategy.page = strategy_page;
    lv_page_manager_add(&lv_page_strategy);

    return;
}