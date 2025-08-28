/**
 * @file lv_robot_weather_ui.c
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
#include <curl/curl.h>
#include "lv_robot_weather_ui.h"
#include "../../generic/cJSON.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
**********************/
typedef struct
{
    char id[16];
    char name[32];
    char country[16];
    char path[64];
    char timezone[32];
    char tz_offset[16];
    char text[16];
    char code[4];
    char temp[8];
    char last_update[32];
} weather_t;

typedef struct weather_data
{
    char memory[1024];
    size_t size;
} weather_data_t;

/**********************
 *  STATIC VARIABLES
 **********************/
static CURL *curl_hd;
static char last_city[16];
static weather_t weather;
static weather_data_t chunk;

static lv_obj_t *dropdown;
static lv_obj_t *weath_info;
static lv_obj_t *weather_page;
static lv_timer_t *timer;
static lv_style_t style;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
LV_IMG_DECLARE(img_app_icon_weather);
void parse_json_weather(char *, weather_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void robot_style_init();
static void timer_api_weather(lv_timer_t *);

static void lv_robot_weather_init();
static void lv_robot_weather_deinit();
static void robot_show_weather_page(void);
static void robot_exit_weather_page(void);
static void robot_weather_page_create(void);
static void drop_down_event_handler(lv_event_t *);
static size_t write_memory_callback(void *, size_t, size_t, void *);

static lv_widget_t lv_page_weather = {
    .page = NULL,
    .name = "天气",
    .type = LV_PAGE_WEATHER,
    .image = &img_app_icon_weather,
    .create_func = robot_weather_page_create,
    .show_func = robot_show_weather_page,
    .exit_func = robot_exit_weather_page,
    .reserved = NULL
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_robot_weather_ui_register()
{
    lv_page_manager_add(&lv_page_weather);
}

void parse_json_weather(char *msg, weather_t *w)
{
    cJSON *json, *ja, *jo, *josub, *item;

    json = cJSON_Parse(msg);
    if (NULL == json) {
        printf("解析json失败");
        return;
    }
    if((ja = cJSON_GetObjectItem(json, "results")) != NULL)
    {
        if((jo = cJSON_GetArrayItem(ja, 0)) != NULL)
        {
            if((josub = cJSON_GetObjectItem(jo, "location")) != NULL)
            {
                if((item = cJSON_GetObjectItem(josub, "id")) != NULL) {
                    lv_memcpy(w->id, item->valuestring, lv_strlen(item->valuestring));
                }
                if((item = cJSON_GetObjectItem(josub, "name")) != NULL) {
                    lv_memcpy(w->name, item->valuestring, lv_strlen(item->valuestring));
                }
                if((item = cJSON_GetObjectItem(josub, "country")) != NULL) {
                    lv_memcpy(w->country, item->valuestring, lv_strlen(item->valuestring));
                }
                if((item = cJSON_GetObjectItem(josub, "path")) != NULL) {
                    lv_memcpy(w->path, item->valuestring, lv_strlen(item->valuestring));
                }
                if((item = cJSON_GetObjectItem(josub, "timezone")) != NULL) {
                    lv_memcpy(w->timezone, item->valuestring, lv_strlen(item->valuestring));
                }
                if((item = cJSON_GetObjectItem(josub, "timezone_offset")) != NULL) {
                    lv_memcpy(w->tz_offset, item->valuestring, lv_strlen(item->valuestring));
                }
            }
            if((josub = cJSON_GetObjectItem(jo, "now")) != NULL)
            {
                if((item = cJSON_GetObjectItem(josub, "text")) != NULL) {
                    lv_memcpy(w->text, item->valuestring, lv_strlen(item->valuestring));
                }
                if((item = cJSON_GetObjectItem(josub, "code")) != NULL) {
                    lv_memcpy(w->code, item->valuestring, lv_strlen(item->valuestring));
                }
                if((item = cJSON_GetObjectItem(josub, "temperature")) != NULL) {
                    lv_memcpy(w->temp, item->valuestring, lv_strlen(item->valuestring));
                }
            }
            if((josub = cJSON_GetObjectItem(jo, "last_update")) != NULL)
            {
                lv_memcpy(w->last_update, josub->valuestring, lv_strlen(josub->valuestring));
            }
        }
    }

    cJSON_Delete(json);
    return;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/
static void robot_style_init()
{
    lv_style_init(&style);
    lv_style_set_radius(&style, 0);
    lv_style_set_pad_all(&style, 0);
    lv_style_set_border_width(&style, 0);

    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_text_font(&style, fzlth_font_30);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_LEFT);
}

static void robot_show_weather_page(void)
{
    //恢复定时器
    lv_timer_resume(timer);

    lv_obj_set_style_bg_color(weather_page, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(weather_page, LV_OPA_COVER, LV_PART_MAIN);

    //创建下拉列表
    dropdown = lv_dropdown_create(weather_page);
    lv_dropdown_set_options(dropdown, "北京\n"
                            "上海\n"
                            "广州\n"
                            "成都\n"
                            "杭州\n"
                            "重庆\n"
                            "南京\n"
                            "无锡\n"
                            "天津\n"
                            "郑州");
    lv_dropdown_set_symbol(dropdown, NULL);
    lv_dropdown_set_selected(dropdown, 0);
    lv_dropdown_set_text(dropdown, "城市");
    lv_obj_set_style_bg_color(dropdown, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(dropdown, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_size(dropdown, 80, LV_SIZE_CONTENT);
    lv_obj_align(dropdown, LV_ALIGN_TOP_RIGHT, 0, 10);
    lv_dropdown_set_dir(dropdown, LV_DIR_BOTTOM);
    lv_obj_add_event_cb(dropdown, drop_down_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(dropdown, &style, LV_PART_MAIN);

    // 获取下拉列表中的列表来修改样式或其他修改(显示中文)
    lv_obj_set_style_text_font(lv_dropdown_get_list(dropdown), fzlth_font_30, 0);

    weath_info = lv_label_create(weather_page);

    //请求天气数据并绘制
    timer_api_weather(NULL);

    return;
}

static void robot_exit_weather_page(void)
{
    //1.销毁或者暂停定时器
    lv_timer_pause(timer);
}

static void robot_weather_page_create(void)
{
    robot_style_init();
    lv_robot_weather_init();

    weather_page = lv_obj_create(NULL);
    lv_obj_add_style(weather_page, &style, 0);
    lv_obj_center(weather_page);

    lv_page_weather.page = weather_page;
    lv_page_manager_add(&lv_page_weather);

    return;
}

static void lv_robot_weather_init()
{
    curl_global_init(CURL_GLOBAL_ALL);
    curl_hd = curl_easy_init();

    sprintf(last_city, "北京");
    //定时请求天气数据(不能阻塞)
    timer = lv_timer_create(timer_api_weather, 10000, &chunk);

    //初始暂停定时器
    lv_timer_pause(timer);

    return;
}

static void lv_robot_weather_deinit()
{
    curl_easy_cleanup(curl_hd);
    curl_global_cleanup();

    return;
}

static void drop_down_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    char buf[16];
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        if (lv_memcmp(buf, last_city, sizeof(last_city)) != 0)
        {
            lv_memcpy(last_city, buf, sizeof(last_city));
            //更新下拉列表文本
            lv_dropdown_set_text(dropdown, last_city);
            //强制渲染下拉
            lv_obj_invalidate(dropdown);
            //请求天气数据并绘制
            timer_api_weather(NULL);
        }
    }
}

static void timer_api_weather(lv_timer_t *timer)
{
    chunk.size = 0;
    lv_memset(&chunk.memory, 0, sizeof(chunk.memory));

    if (NULL != dropdown) {
        lv_dropdown_set_text(dropdown, last_city);
    }

    char buf[1024] = {0};
    sprintf(buf, "https://api.thinkpage.cn/v3/weather/now.json?key=S9qWEP29iQ_dIBum5&location=%s&language=zh-Hans&unit=c", last_city);
    // curl_easy_setopt(curl_hd, CURLOPT_VERBOSE, 1L);//开启调试模式
    curl_easy_setopt(curl_hd, CURLOPT_URL, buf);
    curl_easy_setopt(curl_hd, CURLOPT_WRITEFUNCTION, write_memory_callback);
    curl_easy_setopt(curl_hd, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_hd, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl_hd, CURLOPT_TIMEOUT, 1L);//超时1秒

    CURLcode res = curl_easy_perform(curl_hd);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() 执行失败，原因是 %s\n", curl_easy_strerror(res));
    }
    else
    {
        // printf("接收到网页数据%lu字节 \"%s\"\n\n", (unsigned long)chunk.size, chunk.memory);
        parse_json_weather(chunk.memory, &weather);
    }

    //创建标签显示天气信息
    lv_obj_set_size(weath_info, lv_pct(100), LV_SIZE_CONTENT);
    lv_label_set_text_fmt(weath_info, "城市: %s\n\n"
                            "天气: %s\n"
                            "温度: %s摄氏度\n"
                            "更新时间: %s\n"
                            "时区: %s\n",
                            weather.name, weather.text, weather.temp, weather.last_update, weather.timezone);
    lv_obj_align(weath_info, LV_ALIGN_LEFT_MID, 20, 0);
    lv_obj_add_style(weath_info, &style, LV_PART_MAIN);

    return;
}

static size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    weather_data_t *mem = (weather_data_t *)userp;

    lv_memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}