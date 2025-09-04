/**
 * @file lv_robot_font.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/*********************
 *     INCLUDES
 *********************/
#include <stdio.h>
#include "lv_robot_font.h"

/*********************
 *     DEFINES
 *********************/
lv_font_t *fzlth_font_30;
lv_font_t *fzlth_font_40;
lv_font_t *fzlth_font_100;
lv_font_t *fzlth_font_30B;
lv_font_t *fzlth_font_100B;

#define FZLTH_GB18030L2_B_DIR "../../assert/font/FZLTH_GB18030L2/FZLTH_GB18030L2_B.ttf"
#define FZLTH_GB18030L2_R_DIR "../../assert/font/FZLTH_GB18030L2/FZLTH_GB18030L2_R.ttf"

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
lv_font_t *create_freetype_font(size_t size, lv_freetype_font_style_t style)
{
    const char *path = NULL;

    switch (style)
    {
        case FT_FONT_STYLE_NORMAL: {
            path = FZLTH_GB18030L2_R_DIR;
            break;
        }
        case FT_FONT_STYLE_BOLD: {
            path = FZLTH_GB18030L2_B_DIR;
            break;
        }
        default:
            break;
    }

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        LV_LOG_ERROR("Can't open font file! Check path: %s", path);
        return NULL;
    }
    fclose(fp);

    lv_font_t *font = lv_freetype_font_create(path, LV_FREETYPE_FONT_RENDER_MODE_BITMAP, size, style);
    if (!font)
    {
        LV_LOG_ERROR("freetype font create failed.");
        return NULL;
    }

    return font;
}

void delete_freetype_font(lv_font_t *font)
{
    if (NULL == font) return;

    lv_freetype_font_delete(font);

    return;
}