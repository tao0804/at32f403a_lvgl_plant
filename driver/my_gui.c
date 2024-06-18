#include "my_gui.h"
#include "lvgl.h"
// #include "lv_drivers/win32drv/win32drv.h"

static lv_obj_t *tabview;
static lv_group_t *group1, *group2;
static lv_obj_t *btn1, *btn2, *btn3, *btn4;
static lv_obj_t * ta1;

#define  BATTERY_OUTLINE_W    40 //电池图标宽度
#define  BATTERY_OUTLINE_H    20 //电池图标高度

static void my_gui_battery_cb(void* p, int32_t v)
{
    //实现变色效果，电池电量低于20% 红色
    static int32_t cnt;
    if (cnt >= BATTERY_OUTLINE_W * 0.2 && v < BATTERY_OUTLINE_W * 0.2)
    {
        lv_obj_set_style_bg_color(p, lv_color_hex(0xff0000), 0);
    }
    else if (v >= BATTERY_OUTLINE_W * 0.2 && cnt < BATTERY_OUTLINE_W * 0.2)
    {
        lv_obj_set_style_bg_color(p, lv_color_hex(0xff00), 0);
    }
    cnt = v;

    //修改电量颜色obj宽度
    lv_obj_set_width(p, v);

    //修改电池百分比
    lv_obj_t *text = lv_obj_get_child(lv_obj_get_parent(p), -1);
    lv_label_set_text_fmt(text, "%d", v*100/(BATTERY_OUTLINE_W-4));
}

static void my_gui_battery_init(void)
{
    lv_obj_t* outline = lv_obj_create(lv_scr_act());

    //设置border和pading
    lv_obj_set_style_border_width(outline, 2, 0);
    lv_obj_set_style_pad_all(outline, 0, 0);

    //设置圆角
    lv_obj_set_style_radius(outline, 8, 0);

    //关闭滚动条
    lv_obj_clear_flag(outline, LV_OBJ_FLAG_SCROLLABLE);

    //设置宽高
    lv_obj_set_size(outline, BATTERY_OUTLINE_W, BATTERY_OUTLINE_H);

    //居中对齐
    lv_obj_align(outline, LV_ALIGN_CENTER, 0, 0);

    //电池电量填充obj
    lv_obj_t* pad = lv_obj_create(outline);


    //设置outline
    lv_obj_set_style_outline_width(pad, 0, 0);
    lv_obj_set_style_outline_pad(pad, 0, 0);
    lv_obj_set_style_border_width(pad, 0, 0);
    //设置背景色
    lv_obj_set_style_bg_color(pad, lv_color_hex(0xff0000), 0);

    //设置宽高
    lv_obj_set_size(pad, BATTERY_OUTLINE_W, BATTERY_OUTLINE_H-4);
    lv_obj_set_style_border_width(pad, 0, 0);

    //设置圆角
    lv_obj_set_style_radius(pad, 8, 0);

    //右上显示
    lv_obj_align(outline, LV_ALIGN_TOP_RIGHT, 0, 0);

    //关闭滚动条
    lv_obj_clear_flag(pad, LV_OBJ_FLAG_SCROLLABLE);

    //电池百分比
    lv_obj_t* label = lv_label_create(outline);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    //设置动画， 模仿电池电量变化
    lv_anim_t a;
    lv_anim_init(&a);

    /*Set the "animator" function*/
    lv_anim_set_exec_cb(&a, my_gui_battery_cb);

    /*Set the "animator" function*/
    lv_anim_set_var(&a, pad);

    /*Length of the animation [ms]*/
    lv_anim_set_time(&a, 10000);

    /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_values(&a, 0, BATTERY_OUTLINE_W-4);

    /*Time to wait before starting the animation [ms]*/
    lv_anim_set_delay(&a, 1000);

    /*Play the animation backward too with this duration. Default is 0 (disabled) [ms]*/
    lv_anim_set_playback_time(&a, 0);

    /*Delay before playback. Default is 0 (disabled) [ms]*/
    lv_anim_set_playback_delay(&a, 0);

    /*Number of repetitions. Default is 1.  LV_ANIM_REPEAT_INFINIT for infinite repetition*/
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    /*Delay before repeat. Default is 0 (disabled) [ms]*/
    lv_anim_set_repeat_delay(&a, 1000);

    /* START THE ANIMATION
     *------------------*/
    lv_anim_start(&a);                             /*Start the animation*/
}

#define ARRAY_NUM(arr)			(sizeof(arr)/sizeof((arr)[0]))

struct my_gui_config_value
{
    uint16_t cfg_range;
    uint16_t cfg_value;
    char ** cfg_str;
};

char *cfg_freq[] = {"2430MHz", "2440MHz", "2450MHz"};
char *cfg_rate[] = {"100Kbps","500Kbps","1Mbps"};
char *cfg_percent[] = {"0\%", "10\%", "20\%", "30\%", "40\%", "50\%", "60\%", "70\%", "80\%", "90\%", "100\%"};

static void my_gui_btn_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    uint32_t key = *(uint32_t*)lv_event_get_param(e);
    lv_obj_t *label = lv_obj_get_user_data(btn);    // 获取label对象
    struct my_gui_config_value *cfg = lv_obj_get_user_data(label);    // 获取cfg

    if (event_code != LV_EVENT_KEY) {
        return; // 本回调只处理KEY事件
    }

    if (key == LV_KEY_RIGHT) {
        // 增大配置项
        if (cfg->cfg_value + 1 < cfg->cfg_range) {
            cfg->cfg_value++;
            lv_label_set_text(label, cfg->cfg_str[cfg->cfg_value]);
        }
    } else if (key == LV_KEY_LEFT) {
        // 减小配置项
        if (cfg->cfg_value > 0) {
            cfg->cfg_value--;
            lv_label_set_text(label, cfg->cfg_str[cfg->cfg_value]);
        }
    }
}

static void my_gui_set_group(lv_group_t *g)
{
    if (g == group1) {
        lv_group_remove_all_objs(group1);
        lv_group_add_obj(group1, lv_tabview_get_tab_btns(tabview));     // tableview 按键也加进group
        lv_group_add_obj(group1, btn1);
        lv_group_add_obj(group1, btn2);
        lv_group_add_obj(group1, btn3);
        lv_group_add_obj(group1, btn4);
        lv_group_add_obj(group1, ta1);
        // lv_indev_set_group(lv_win32_keypad_device_object, group1);     // 将键盘和组1关联
    } else if (g == group2) {
        lv_group_remove_all_objs(group2);
        lv_group_add_obj(group2, lv_tabview_get_tab_btns(tabview));
        // lv_indev_set_group(lv_win32_keypad_device_object, group2);     // 将键盘和组2关联
    }
}

static void my_gui_tv_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_tabview_t *tv = (lv_tabview_t *)tabview;
    uint32_t key = *(uint32_t*)lv_event_get_param(e);
    uint8_t act_id;

    if (event_code != LV_EVENT_KEY) {
        return; // 本回调只处理KEY事件
    }

    if (key == LV_KEY_RIGHT) {
        // 右移tabview
        if (tv->tab_cur + 1 < tv->tab_cnt) {
            act_id = tv->tab_cur + 1;
        } else {
            act_id = 0;
        }
        LV_LOG_USER("right, cur:%d, next:%d", tv->tab_cur, act_id);
    } else if (key == LV_KEY_LEFT) {
        // 左移tabview
        if (tv->tab_cur > 0) {
            act_id = tv->tab_cur - 1;
        } else {
            act_id = tv->tab_cnt - 1;
        }
        LV_LOG_USER("left, cur:%d, next:%d", tv->tab_cur, act_id);
    }

    if (act_id == 0) {
        my_gui_set_group(group1);
    } else {
        my_gui_set_group(group2);
    }
    lv_tabview_set_act((lv_obj_t *)tv, act_id, LV_ANIM_OFF);    // 设置完group后再切，否则存在问题，不能开启动画，否则动画帧有focus问题
}

static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);
    LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}

static void my_gui_tabview_init(void)
{
    /*Create a Tab view object*/
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 40);  // tab_size 设置选项卡高度
    lv_obj_set_size(tabview, 240, 300);  // 设置整体大小
    lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, 20);

#if 1
    // tabview中增加一个tab
    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Config");
    lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_OFF);     // 关滚动栏

    // 创建样式，应用于list，直角边，透明背景，无边界值
    static lv_style_t style;    // 需要为static，不然不起作用
    lv_style_init(&style);
    lv_style_set_radius(&style, 0);
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP); // 设置透明背景
    lv_style_set_border_width(&style, 0);

    /*Add content to the tab1*/
    lv_obj_t * list1;
    list1 = lv_list_create(tab1);
    lv_obj_set_size(list1, 240, 260);
    lv_obj_align(list1, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(list1, &style, 0);

    // 创建按键
    btn1 = lv_list_add_btn(list1, NULL, "Freq:");
    lv_obj_add_event_cb(btn1, my_gui_btn_cb, LV_EVENT_KEY, NULL);

    btn2 = lv_list_add_btn(list1, NULL, "Rate:");
    lv_obj_add_event_cb(btn2, my_gui_btn_cb, LV_EVENT_KEY, NULL);

    btn3 = lv_list_add_btn(list1, NULL, "Volume:");
    lv_obj_add_event_cb(btn3, my_gui_btn_cb, LV_EVENT_KEY, NULL);

    btn4 = lv_list_add_btn(list1, NULL, "Brightness:");
    lv_obj_add_event_cb(btn4, my_gui_btn_cb, LV_EVENT_KEY, NULL);

    // 创建textarea
    lv_obj_t *  btn5 = lv_list_add_btn(list1, NULL, "Code:");
    ta1 = lv_textarea_create(btn5);
    lv_textarea_set_one_line(ta1, true);
    lv_textarea_set_accepted_chars(ta1, "0123456789");   // 设置仅接受数字字符
    lv_obj_align(ta1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_width(ta1, 100);
    lv_obj_add_event_cb(ta1, textarea_event_handler, LV_EVENT_READY, NULL);
    lv_obj_clear_state(ta1, LV_STATE_FOCUSED);

    // 创建group
    group1 = lv_group_create();
    my_gui_set_group(group1);    // 设置group

    // 创group2，tabview按键回调
    group2 = lv_group_create();
    lv_obj_add_event_cb(lv_tabview_get_tab_btns(tabview), my_gui_tv_cb, LV_EVENT_KEY, NULL);

    // 创建config值 label
    static lv_obj_t * label1;
    static struct my_gui_config_value cfg1 = {.cfg_range = ARRAY_NUM(cfg_freq), .cfg_value = 1, cfg_freq};
    label1 = lv_label_create(btn1);
    lv_label_set_text(label1, cfg1.cfg_str[cfg1.cfg_value]);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_flex_grow(label1, 1);
    lv_obj_set_user_data(btn1, label1); // 将label对象关联到btn对象，减少全局变量使用
    lv_obj_set_user_data(label1, &cfg1); // 将cfg数据关联到label对象，减少全局变量使用

    static lv_obj_t * label2;
    static struct my_gui_config_value cfg2 = {.cfg_range = ARRAY_NUM(cfg_rate), .cfg_value = 1, cfg_rate};
    label2 = lv_label_create(btn2);
    lv_label_set_text(label2, cfg2.cfg_str[cfg2.cfg_value]);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_flex_grow(label2, 1);
    lv_obj_set_user_data(btn2, label2);
    lv_obj_set_user_data(label2, &cfg2);

    static lv_obj_t * label3;
    static struct my_gui_config_value cfg3 = {.cfg_range = ARRAY_NUM(cfg_percent), .cfg_value = 5, cfg_percent};
    label3 = lv_label_create(btn3);
    lv_label_set_text(label3, cfg3.cfg_str[cfg3.cfg_value]);
    lv_label_set_long_mode(label3, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_flex_grow(label3, 1);
    lv_obj_set_user_data(btn3, label3);
    lv_obj_set_user_data(label3, &cfg3);

    static lv_obj_t * label4;
    static struct my_gui_config_value cfg4 = {.cfg_range = ARRAY_NUM(cfg_percent), .cfg_value = 5, cfg_percent};
    label4 = lv_label_create(btn4);
    lv_label_set_text(label4, cfg4.cfg_str[cfg4.cfg_value]);
    lv_label_set_long_mode(label4, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_flex_grow(label4, 1);
    lv_obj_set_user_data(btn4, label4);
    lv_obj_set_user_data(label4, &cfg4);
#endif

#if 1
    // tabview中增加一个tab
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Comm");
    lv_obj_set_scrollbar_mode(tab2, LV_SCROLLBAR_MODE_OFF);

    /*Add content to the tab2*/
    lv_obj_t * table = lv_table_create(tab2);
    lv_group_remove_obj(table); // 不分配group
    lv_table_set_cell_value(table, 0, 0, "ISSI");
    lv_table_set_cell_value(table, 0, 1, "0db");
    lv_table_set_cell_value(table, 1, 0, "TXCnt");
    lv_table_set_cell_value(table, 1, 1, "0");
    lv_table_set_cell_value(table, 2, 0, "RXCnt");
    lv_table_set_cell_value(table, 2, 1, "0");

    /*Set a smaller height to the table. It'll make it scrollable*/
    lv_table_set_col_width(table, 0, 105);
    lv_table_set_col_width(table, 1, 105);
    // lv_obj_set_height(table, 220);
    lv_obj_align(table, LV_ALIGN_TOP_MID, 0, 0);
#endif

#if 1
    // tabview中增加一个tab
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "About");
    lv_obj_set_scrollbar_mode(tab3, LV_SCROLLBAR_MODE_OFF);

    /*Add content to the tab3*/
    lv_obj_t * label = lv_label_create(tab3);
    lv_label_set_text(label, "Version: 0.0.2");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);
#endif
}

void my_gui_init(void)
{
    my_gui_tabview_init();
    my_gui_battery_init();
}
