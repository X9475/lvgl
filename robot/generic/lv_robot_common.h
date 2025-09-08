/**
 * @file lv_robot_common.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-09-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LV_ROBOT_COMMON_H
#define LV_ROBOT_COMMON_H

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

/**********************
 * GLOBAL FUNCTIONS
 **********************/

/// @brief 创建二维码对象
lv_obj_t *lv_qrcode_create_func(lv_obj_t *, void *);

/**********************
 * MACRO FUNCTIONS
 **********************/

#ifdef __cplusplus
}
#endif

#endif /*LV_ROBOT_COMMON_H*/