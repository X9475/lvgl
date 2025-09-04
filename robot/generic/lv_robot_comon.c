/**
 * @file lv_robot_comon.c
 * @author your name (you@domain.com)
 * @brief 通用功能函数
 * @version 0.1
 * @date 2025-09-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/*********************
 *     INCLUDES
 *********************/
#include "lv_robot_comon.h"

/*********************
 *     DEFINES
 *********************/

/**********************
 *     TYPEDEFS
 **********************/

/**********************
 * STATIC VARIABLES
 **********************/

/**********************
 * STATIC PROTOTYPES
 **********************/

/**********************
 * GLOBAL FUNCTIONS
 **********************/
lv_obj_t *lv_qrcode_create_func(lv_obj_t *parent, void *data)
{
    lv_obj_t *qr = lv_qrcode_create(parent);

    const char *qrdata = (const char *)data;
    lv_qrcode_update(qr, qrdata, strlen(qrdata));

    return qr;
}
