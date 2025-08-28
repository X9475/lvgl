/**
 * @file lv_robot_generic.c
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
#include "lv_robot_generic.h"

/*********************
 *     DEFINES
 *********************/

/**********************
 *     TYPEDEFS
 **********************/

/**********************
 * STATIC PROTOTYPES
 **********************/

/**********************
 * STATIC VARIABLES
 **********************/
static lv_widget_stack_t *stack; //页面栈
static lv_robot_page_manager_t *manager; //页面管理器

/**********************
 *  GLOBAL FUNCTIONS
 **********************/
void lv_manager_init()
{
    lv_page_stack_init();
    lv_page_manager_init();

    //freetype font create
    fzlth_font_30 = create_freetype_font(30, LV_FREETYPE_FONT_STYLE_NORMAL);
    fzlth_font_40 = create_freetype_font(40, LV_FREETYPE_FONT_STYLE_NORMAL);
    fzlth_font_100 = create_freetype_font(100, LV_FREETYPE_FONT_STYLE_NORMAL);

    return;
}

void lv_manager_deinit()
{
    lv_page_stack_deinit();
    lv_page_manager_deinit();

    //freetype font delete
    delete_freetype_font(fzlth_font_30);
    delete_freetype_font(fzlth_font_40);
    delete_freetype_font(fzlth_font_100);

    return;
}

/***********************************页面资源管理*********************************/
void lv_page_manager_init()
{
    manager = (lv_robot_page_manager_t *)lv_malloc(sizeof(lv_robot_page_manager_t));
    LV_ASSERT_MALLOC(manager);

    manager->page_list = (lv_ll_t *)lv_malloc(sizeof(lv_ll_t));
    LV_ASSERT_MALLOC(manager->page_list);
    if (NULL != manager->page_list)
    {
        lv_ll_init(manager->page_list, sizeof(lv_widget_t));
    }

    return;
}

void lv_page_manager_deinit()
{
    if (NULL == manager) return;

    lv_ll_clear(manager->page_list);  //清空链表
    lv_free(manager); manager = NULL;

    return;
}

void lv_page_manager_add(lv_widget_t *widget)
{
    if (NULL == manager->page_list) return;

    //查找是否已经存在
    void *page_ptr = lv_page_manager_foreach(manager->page_list, widget->type);
    if (NULL != page_ptr)
    {
        //已经加入过的页面信息，只更新容器指针
        ((lv_widget_t *)page_ptr)->page = widget->page;
        ((lv_widget_t *)page_ptr)->reserved = widget->reserved;
    }
    else
    {
        lv_widget_t *new_node = (lv_widget_t *)lv_ll_ins_tail(manager->page_list);
        lv_memset(new_node, 0, sizeof(lv_widget_t));
        lv_memcpy(new_node, widget, sizeof(lv_widget_t));
    }

    return;
}

void *lv_page_manager_change(uint8_t type)
{
    if (NULL == manager->page_list) return NULL;

    //查找是否已经存在
    void *page_ptr = lv_page_manager_foreach(manager->page_list, type);
    if (NULL == page_ptr) {
        LV_LOG_USER("page not found");
        return NULL;
    }

    lv_widget_t *widget = (lv_widget_t *)page_ptr;
    widget->show_func();//屏幕显示

    //NOTE:防止页面回退时再次加入栈内导致重复显示，菜单不计入返回页面栈
    lv_widget_t *top = lv_page_stack_top();
    if ((top != widget) && (type != LV_PAGE_MENU))
    {
        lv_page_stack_push(widget);
    }

    return (void *)widget;
}

void *lv_page_foreach_find(uint8_t type)
{
    if (NULL == manager->page_list) return NULL;

    void *page_ptr = lv_page_manager_foreach(manager->page_list, type);
    if (NULL != page_ptr)
    {
        return page_ptr;
    }

    return NULL;
}

void *lv_page_manager_foreach(lv_ll_t *list, uint8_t type)
{
    void *node = lv_ll_get_head(list);

    while (node != NULL)
    {
        lv_widget_t *widget = (lv_widget_t *)node;
        if (widget->type == type)
        {
            return node;
        }
        node = lv_ll_get_next(list, node);
    }

    return NULL;
}

void *lv_find_widget_from_page(lv_obj_t *page)
{
    if (NULL == manager->page_list) return NULL;

    void *node = lv_ll_get_head(manager->page_list);

    while (node != NULL)
    {
        lv_widget_t *widget = (lv_widget_t *)node;
        if (widget->page == page)
        {
            return node;
        }
        node = lv_ll_get_next(manager->page_list, node);
    }

    return NULL;
}

/***********************************页面顺序管理*********************************/
void lv_page_stack_init()
{
    stack = (lv_widget_stack_t *)lv_malloc(sizeof(lv_widget_stack_t));
    LV_ASSERT_MALLOC(stack);

    stack->top = NULL;
    stack->count = 0;
    pthread_mutex_init(&stack->lock, NULL);

    return;
}

void *lv_page_stack_top()
{
    return stack->top ? (void *)stack->top->widget : NULL;
}

void *lv_page_stack_pop()
{
    pthread_mutex_lock(&stack->lock);
    if (NULL == stack->top) {
        pthread_mutex_unlock(&stack->lock);
        return NULL;
    }
    struct Node *tmp = stack->top;
    stack->top = tmp->next;
    tmp->next = NULL;
    tmp->widget = NULL;
    stack->count--;
    lv_free(tmp);
    pthread_mutex_unlock(&stack->lock);

    return (void *)stack->top->widget;
}

void lv_page_stack_push(lv_widget_t *widget)
{
    struct Node *new = lv_malloc(sizeof(struct Node));
    if (NULL == new) return;

    pthread_mutex_lock(&stack->lock);
    new->widget = widget;
    new->next = stack->top;
    stack->top = new;
    stack->count++;
    pthread_mutex_unlock(&stack->lock);

    return;
}

bool lv_page_stack_exsit(uint8_t type)
{
    struct Node *top = stack->top;

    if (top->widget->type == type)
    {
        return true;
    }

    return false;
}

void lv_page_stack_deinit()
{
    while (stack->top)
    {
        struct Node *temp = stack->top;
        stack->top = temp->next;
        lv_free(temp);
    }

    pthread_mutex_destroy(&stack->lock);
    lv_free(stack); stack = NULL;

    return;
}

/***********************************屏幕滑动处理*********************************/
void lv_page_manager_gesture_event(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_widget_t *data = lv_obj_get_user_data(obj);

    lv_widget_t *cur_widget = lv_page_stack_top();//当前展示页面
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    lv_indev_wait_release(lv_indev_get_act());

    //LV_LOG_USER("===>dir: %d", dir);
    if (LV_DIR_BOTTOM == dir)
    {//下拉屏幕上边缘

        //下拉只有一个菜单，如果当前是菜单不做动作
        if (cur_widget->type == LV_PAGE_MENU) return;

        lv_widget_t *cur_screen = lv_page_manager_change(LV_PAGE_MENU);
        lv_scr_load_anim(cur_screen->page, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);

        //页面回收前的操作
        if (NULL != cur_widget->exit_func)
        {
            cur_widget->exit_func();
        }

        //清空当前页面内容，保证下次进入页面干净
        lv_obj_clean(cur_widget->page);

        //隐藏底部横条
        lv_obj_add_flag(buttom_line, LV_OBJ_FLAG_HIDDEN);
    }
    else if (LV_DIR_TOP == dir)
    {//上滑屏幕下边缘

        //获取当前活动屏幕对象指针，判断是否为菜单页面
        lv_obj_t *cur_screen = lv_scr_act();

        //上滑底部退出当前页面，如果是桌面则不做动作
        if (cur_widget->type == LV_PAGE_DESKTOP && cur_screen == cur_widget->page) return;

        //隐藏底部横条
        lv_obj_add_flag(buttom_line, LV_OBJ_FLAG_HIDDEN);

        //页面回收前的操作
        if (NULL != cur_widget->exit_func)
        {
            cur_widget->exit_func();
        }

        void *page_ptr = lv_page_foreach_find(LV_PAGE_MENU);
        if (NULL != page_ptr)
        {
            lv_widget_t *widget = (lv_widget_t *)page_ptr;
            if (widget->page == cur_screen)//当前屏幕是菜单
            {
                lv_obj_clean(widget->page);
                lv_widget_t *top = lv_page_stack_top();
                lv_page_manager_change(top->type);
                lv_scr_load_anim(top->page, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
                return;
            }
        }

        //对于确认返回的页面，回收屏幕子控件，但保留屏幕本身
        lv_obj_clean(cur_widget->page);

        lv_widget_t *top_widget = lv_page_stack_pop();
        lv_page_manager_change(top_widget->type);
        lv_scr_load_anim(top_widget->page, LV_SCR_LOAD_ANIM_NONE, 500, 0, false);
    }

    return;
}