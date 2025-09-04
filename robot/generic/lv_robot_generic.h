/**
 * @file lv_robot_generic.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LV_ROBOT_GENERIC_H
#define LV_ROBOT_GENERIC_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *     INCLUDES
 *********************/
#include <pthread.h>
#include "lv_robot_font.h"

/*********************
 *     DEFINES
 *********************/
#define LV_ROBOT_MENU_ITEM_COUNT 11

/**********************
 *     TYPEDEFS
 **********************/
typedef enum
{
    LV_PAGE_UNKNOW = 0xFF,
    LV_PAGE_SETTING = 0,
    LV_PAGE_LUCKCARD,
    LV_PAGE_MYSTERY,
    LV_PAGE_CAMERA,
    LV_PAGE_RECORD,
    LV_PAGE_WEATHER,
    LV_PAGE_STANDSTYLE,
    LV_PAGE_STRATEGY,
    LV_PAGE_RELAX,//最后

    LV_PAGE_DESKTOP,
    LV_PAGE_MENU,
} LV_ROBOT_PAGE;

/// @brief 页面管理结构体
typedef struct
{
    lv_obj_t        *page;
    char            *name;
    uint8_t         type;
    char            *path;
    void            *reserved;

    void ( *show_func)();
    void ( *exit_func)();
    void ( *create_func)();
} lv_widget_t;

typedef struct
{
    lv_ll_t *page_list;
} lv_robot_page_manager_t;

/// @brief 栈模型结构体
struct Node
{
    lv_widget_t *widget;
    struct Node *next;
};

typedef struct
{
    uint32_t count;
    struct Node *top;
    pthread_mutex_t lock;
} lv_widget_stack_t;

/**********************
 * GLOBAL VARIABLES
 **********************/
extern lv_obj_t *buttom_line;

/**********************
 * GLOBAL FUNCTIONS
 **********************/

/// @brief 资源管理器初始化
void lv_manager_init();

/// @brief 资源管理器销毁
void lv_manager_deinit();

void lv_page_manager_init();

void lv_page_manager_deinit();

void lv_page_manager_add(lv_widget_t *);

void *lv_page_manager_change(uint8_t);

void *lv_page_foreach_find(uint8_t);

void *lv_page_manager_foreach(lv_ll_t *, uint8_t);

void *lv_find_widget_from_page(lv_obj_t *);

void lv_page_stack_init();

void *lv_page_stack_top();

void *lv_page_stack_pop();

void lv_page_stack_push(lv_widget_t *);

bool lv_page_stack_exsit(uint8_t);

void lv_page_stack_deinit();

void lv_page_manager_gesture_event(lv_event_t *);

/**********************
 * MACRO FUNCTIONS
 **********************/

#ifdef __cplusplus
}
#endif

#endif /*LV_ROBOT_GENERIC_H*/