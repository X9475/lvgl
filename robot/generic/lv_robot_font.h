/**
 * @file lv_robot_font.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LV_ROBOT_FONT_H
#define LV_ROBOT_FONT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *     INCLUDES
 *********************/
#include "../../lvgl.h"

/*********************
 *     DEFINES
 *********************/

/**********************
 *     TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/
extern lv_font_t *fzlth_font_30;
extern lv_font_t *fzlth_font_40;
extern lv_font_t *fzlth_font_100;

/**********************
 * GLOBAL FUNCTIONS
 **********************/
void delete_freetype_font(lv_font_t *font);

lv_font_t *create_freetype_font(size_t size, lv_freetype_font_style_t style);

/**********************
 * MACRO FUNCTIONS
 **********************/

#ifdef __cplusplus
}
#endif

#endif /*LV_ROBOT_FONT_H*/