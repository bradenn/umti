//
// Created by Braden Nicholson on 3/31/26.
//


#include <cmath>
#include "screen_main.h"
#include "lvgl.h"


lv_obj_t *dock_home;
lv_obj_t *dock_apps;
lv_obj_t *dock_settings;

lv_obj_t *um_body;
lv_obj_t *um_body_current;

lv_obj_t *view_home;
lv_obj_t *view_apps;
lv_obj_t *view_settings;

lv_obj_t *header_wifi;
lv_obj_t *header_time;
lv_obj_t *header_date;

void um_wifi_set(void *params) {
    if (params == nullptr) {
        return;
    }

    if (header_wifi == nullptr) {
        return;
    }

    uint8_t value = *(uint8_t *) (params);
    switch (value) {
        case 0:
            lv_image_set_src(header_wifi, &wifi_1_icon);
            break;
        case 1:
            lv_image_set_src(header_wifi, &wifi_2_icon);
            break;
        case 2:
            lv_image_set_src(header_wifi, &wifi_3_icon);
            break;
        default:
            lv_image_set_src(header_wifi, &wifi_1_icon);
            break;
    }
}

const char *monthNames[12] = {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec",
};

const char *weekDays[7] = {
        "Sun",
        "Mon",
        "Tues",
        "Wed",
        "Thur",
        "Fri",
        "Sat",
};

void um_time_set(void *params) {
    if (params == nullptr) {
        return;
    }

    if (header_time == nullptr || header_date == nullptr) {
        return;
    }

//    // 8:32:17 PM = (12+8)*(60*60)+(32*60)+17 = 73937
//    struct tm *value = (struct tm *) (params);
//
//    lv_label_set_text_fmt(header_date, "%s %s %d", weekDays[value->tm_wday], monthNames[value->tm_mon], value->tm_mday);
//
//    int hour = value->tm_hour == 0 ? 12 : value->tm_hour % 12;
//
//    lv_label_set_text_fmt(header_time, "%d:%02d:%02d %s", hour, value->tm_min, value->tm_sec,
//                          value->tm_hour >= 12 ? "PM" : "AM");

}

void set_view(int view) {
    if (um_body_current == nullptr) return;

    // Hide the old view
    lv_obj_add_flag(um_body_current, LV_OBJ_FLAG_HIDDEN);

    // Select new view
    switch (view) {
        case 0:
            um_body_current = view_home;
            break;
        case 1:
            um_body_current = view_apps;
            break;
        case 2:
            um_body_current = view_settings;
            break;
        default:
            return;
    }

    // Show the new view
    lv_obj_clear_flag(um_body_current, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(um_body_current);   // ensure it's on top inside um_body

    // Force refresh
    lv_obj_invalidate(um_body_current);
    lv_obj_invalidate(um_body);
    lv_obj_update_layout(um_body);

    lv_obj_invalidate(lv_screen_active());
}

static void async_set_view(void *param) {
    set_view((int) (intptr_t) param);
}

lv_obj_t *um_menu_item(lv_obj_t *parent, const char *text) {

    lv_obj_t *menu_item = lv_obj_create(parent);
    lv_obj_set_size(menu_item, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(menu_item, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(menu_item, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *label = lv_label_create(menu_item);
    lv_label_set_text(label, text);

    return menu_item;
}

lv_obj_t *um_build_icon_button(lv_obj_t *parent, um_icon_t iconEnum) {

    lv_obj_t *obj = lv_button_create(parent);
    lv_obj_add_style(obj, &um_button_style, 0);
    lv_obj_add_style(obj, &um_button_active_style, LV_STATE_USER_1);

    lv_obj_t *icon = lv_image_create(obj);

    switch (iconEnum) {
        case ICON_HOME:
            lv_image_set_src(icon, &home_icon);
            break;
        case ICON_APPS:
            lv_image_set_src(icon, &apps_icon);
            break;
        case ICON_SETTINGS:
            lv_image_set_src(icon, &settings_icon);
            break;
        default:
            break;
    }

    lv_image_set_antialias(icon, true);
    lv_obj_set_style_image_recolor(icon, fg4, 0);
    lv_obj_set_style_image_recolor_opa(icon, LV_OPA_100, 0);
    lv_obj_center(icon);

    return obj;
}

void um_dock_button_cb(lv_event_t *e);

void um_dock_button_cb(lv_event_t *e) {
    auto *btn = (lv_obj_t *) lv_event_get_user_data(e);

    // Set all to fall, break before make
    lv_obj_set_state(dock_home, LV_STATE_USER_1, false);
    lv_obj_set_state(dock_apps, LV_STATE_USER_1, false);
    lv_obj_set_state(dock_settings, LV_STATE_USER_1, false);

    if (btn == dock_home) {
        lv_obj_set_state(dock_home, LV_STATE_USER_1, true);
        lv_async_call(async_set_view, (void *) (intptr_t) 0);
    } else if (btn == dock_apps) {
        lv_obj_set_state(dock_apps, LV_STATE_USER_1, true);
        lv_async_call(async_set_view, (void *) (intptr_t) 1);
    } else if (btn == dock_settings) {
        lv_obj_set_state(dock_settings, LV_STATE_USER_1, true);
        lv_async_call(async_set_view, (void *) (intptr_t) 2);
    }
}

static lv_obj_t *slider_label;

void um_slider_button_cb(lv_event_t *e) {
    auto *btn = (lv_obj_t *) lv_event_get_user_data(e);

    lv_label_set_text_fmt(slider_label, "%ld", lv_slider_get_value(btn));
}

static uint16_t mono_adv_w = 0;

static bool fix_w_get_glyph_dsc(const lv_font_t *font, lv_font_glyph_dsc_t *dsc, uint32_t letter,
                                uint32_t letter_next) {
    bool ret = lv_font_get_glyph_dsc_fmt_txt(font, dsc, letter, letter_next);
    if (!ret) return false;

    /* Only fix width for digit characters */
    if (letter >= '0' && letter <= '9') {
        int letterWidth = dsc->adv_w;
        dsc->adv_w = mono_adv_w;
        dsc->ofs_x = (short) round((mono_adv_w - letterWidth) / 2.0);
    }

    return true;
}

lv_obj_t *um_draw_header(lv_obj_t *screen) {

    lv_font_glyph_dsc_t g;
    uint16_t max_w = 0;
    for (char c = '0'; c <= '9'; c++) {
        if (lv_font_get_glyph_dsc_fmt_txt(&lv_font_montserrat_14, &g, c, 0)) {
            if (g.adv_w > max_w) {
                max_w = g.adv_w;
            }
        }
    }
    mono_adv_w = max_w;  /* add 1px (in 1/16 units) of padding each side */

    static lv_font_t mono_font;
    mono_font = lv_font_montserrat_14;
    mono_font.get_glyph_dsc = fix_w_get_glyph_dsc;


    // Dock Grid

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    // Dock

    lv_obj_t *header = lv_obj_create(screen);
    lv_obj_set_size(header, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_pad_hor(header, 8, 0);

    lv_obj_add_style(header, &um_container_style, 0);

    lv_obj_set_grid_dsc_array(header, col_dsc, row_dsc);

    // TITLE

    lv_obj_t *titleBlock = lv_obj_create(header);
    lv_obj_set_grid_cell(titleBlock, LV_GRID_ALIGN_STRETCH, (int32_t) 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    lv_obj_set_flex_flow(titleBlock, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(titleBlock, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_size(titleBlock, LV_SIZE_CONTENT, lv_pct(100));
    lv_obj_add_style(titleBlock, &um_container_style, 0);

    lv_obj_t *headerTitle = lv_label_create(titleBlock);
    lv_obj_set_style_text_font(headerTitle, &roboto_black_18, 0);
    lv_label_set_text(headerTitle, "UDAP");
    lv_obj_set_align(headerTitle, LV_ALIGN_LEFT_MID);

    // TIME

    lv_obj_t *timeBlock = lv_obj_create(header);
    lv_obj_set_grid_cell(timeBlock, LV_GRID_ALIGN_STRETCH, (int32_t) 2, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    lv_obj_set_flex_flow(timeBlock, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(timeBlock, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_size(timeBlock, LV_SIZE_CONTENT, lv_pct(100));
    lv_obj_add_style(timeBlock, &um_container_style, 0);

//    lv_style_set_text_font(&um_header_style, &mono_font);
    header_date = lv_label_create(timeBlock);

    lv_obj_add_style(header_date, &um_header_style, 0);
    lv_label_set_text_fmt(header_date, "12:00:00");

    lv_obj_center(header_date);

    header_time = lv_label_create(timeBlock);

    lv_label_set_text_fmt(header_time, "12:00:00");
    lv_obj_add_style(header_time, &um_header_style, 0);
    lv_obj_center(header_time);


    // ICONS

    lv_obj_t *iconBlock = lv_obj_create(header);
    lv_obj_set_grid_cell(iconBlock, LV_GRID_ALIGN_STRETCH, (int32_t) 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    lv_obj_set_flex_flow(iconBlock, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(iconBlock, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_size(iconBlock, LV_SIZE_CONTENT, lv_pct(100));
    lv_obj_add_style(iconBlock, &um_container_style, 0);

    header_wifi = lv_image_create(iconBlock);
    lv_obj_set_style_transform_scale(header_wifi, 144, 0);
    lv_obj_center(header_wifi);
    lv_obj_set_align(header_wifi, LV_ALIGN_CENTER);

    uint8_t val = 0;
    um_wifi_set(&val);

    return header;
}

lv_obj_t *um_draw_dock(lv_obj_t *screen) {

    // Dock Grid

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    // Dock

    lv_obj_t *dock = lv_obj_create(screen);
    lv_obj_set_size(dock, lv_pct(100), 86);
    lv_obj_center(dock);
    lv_obj_add_style(dock, &um_dock_style, 0);
    lv_obj_set_grid_dsc_array(dock, col_dsc, row_dsc);

    // Buttons

    dock_home = um_build_icon_button(dock, ICON_HOME);
    lv_obj_set_grid_cell(dock_home, LV_GRID_ALIGN_STRETCH, (int32_t) 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(dock_home, um_dock_button_cb, LV_EVENT_CLICKED, (void *) dock_home);
    lv_obj_set_state(dock_home, LV_STATE_USER_1, true);

    dock_apps = um_build_icon_button(dock, ICON_APPS);
    lv_obj_set_grid_cell(dock_apps, LV_GRID_ALIGN_STRETCH, (int32_t) 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(dock_apps, um_dock_button_cb, LV_EVENT_CLICKED, (void *) dock_apps);

    dock_settings = um_build_icon_button(dock, ICON_SETTINGS);
    lv_obj_set_grid_cell(dock_settings, LV_GRID_ALIGN_STRETCH, (int32_t) 2, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(dock_settings, um_dock_button_cb, LV_EVENT_CLICKED, (void *) dock_settings);


    return dock;

}

void um_switch(lv_obj_t *parent, const char* text, const char* description) {

    // Item 1

    lv_obj_t *item1 = lv_obj_create(parent);
    lv_obj_add_style(item1, &um_container_style, 0);
    lv_obj_set_height(item1, 72);
    lv_obj_set_width(item1, lv_pct(100));
    lv_obj_set_flex_flow(item1, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(item1, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_BETWEEN);

    lv_obj_t *itemTitle1 = lv_obj_create(item1);
    lv_obj_add_style(itemTitle1, &um_container_style, 0);
    lv_obj_set_height(itemTitle1, 72);
    lv_obj_set_width(itemTitle1, lv_pct(100));
    lv_obj_set_flex_flow(itemTitle1, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(itemTitle1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    lv_obj_t *label1 = lv_label_create(itemTitle1);
    lv_label_set_text(label1, text);
    lv_obj_set_height(label1, 18);
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(label1, fg1, 0);

    lv_obj_t *description1 = lv_label_create(itemTitle1);
    lv_label_set_text(description1, description);
    lv_obj_set_height(description1, 14);
    lv_obj_set_style_text_font(description1, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(description1, fg4, 0);

    lv_obj_t *sw1 = lv_switch_create(item1);
    lv_obj_center(sw1);


    // Background Style

    int knobPadding = 4;
    int outerBorderRadius = 12;
    int knobRadius = outerBorderRadius - knobPadding;

    static lv_style_t um_switch_main_style;

    lv_style_init(&um_switch_main_style);
    // Background
    lv_style_set_bg_opa(&um_switch_main_style, LV_OPA_COVER);
    lv_style_set_bg_color(&um_switch_main_style, bg1);
    // Border
    lv_style_set_border_opa(&um_switch_main_style, LV_OPA_COVER);
    lv_style_set_border_color(&um_switch_main_style, bg4);
    lv_style_set_border_width(&um_switch_main_style, 2);
    // Geometry
    lv_style_set_pad_all(&um_switch_main_style, -2);
    lv_style_set_radius(&um_switch_main_style, outerBorderRadius);

    lv_style_set_width(&um_switch_main_style, 48 * 2);
    lv_style_set_height(&um_switch_main_style, 48);

    // Indicator Style

    static lv_style_t um_switch_indicator_active_style;

    lv_style_init(&um_switch_indicator_active_style);
    // Background
    lv_style_set_bg_opa(&um_switch_indicator_active_style, LV_OPA_COVER);
    lv_style_set_bg_color(&um_switch_indicator_active_style, bg3);
    // Border
    lv_style_set_border_opa(&um_switch_indicator_active_style, LV_OPA_COVER);
    lv_style_set_border_color(&um_switch_indicator_active_style, bg4);
    lv_style_set_border_width(&um_switch_indicator_active_style, 2);
    // Geometry
    lv_style_set_pad_all(&um_switch_indicator_active_style, 0);
    lv_style_set_radius(&um_switch_indicator_active_style, outerBorderRadius);

    static lv_style_t um_switch_indicator_checked_style;

    lv_style_init(&um_switch_indicator_checked_style);
    // Background
    lv_style_set_bg_opa(&um_switch_indicator_checked_style, LV_OPA_COVER);
    lv_style_set_bg_color(&um_switch_indicator_checked_style, lv_color_make(49, 109, 202));
    // Border
    lv_style_set_border_opa(&um_switch_indicator_checked_style, LV_OPA_COVER);
    lv_style_set_border_color(&um_switch_indicator_checked_style, lv_color_make(49, 109, 202));
    lv_style_set_border_width(&um_switch_indicator_checked_style, 2);
    // Geometry
    lv_style_set_pad_all(&um_switch_indicator_checked_style, 0);
    lv_style_set_radius(&um_switch_indicator_checked_style, outerBorderRadius);

    // Knob Style

    static lv_style_t um_switch_knob_style;

    lv_style_init(&um_switch_knob_style);
    // Background
    lv_style_set_bg_opa(&um_switch_knob_style, LV_OPA_COVER);
    lv_style_set_bg_color(&um_switch_knob_style, bg3);
    // Border
    lv_style_set_border_opa(&um_switch_knob_style, LV_OPA_COVER);
    lv_style_set_border_color(&um_switch_knob_style, bg4);
    lv_style_set_border_width(&um_switch_knob_style, 2);
    // Geometry
    lv_style_set_pad_ver(&um_switch_knob_style, -knobPadding);
    lv_style_set_pad_hor(&um_switch_knob_style, -knobPadding);
    lv_style_set_radius(&um_switch_knob_style, knobRadius);

    static lv_style_t um_switch_knob_checked_style;

    lv_style_init(&um_switch_knob_checked_style);
    // Background
    lv_style_set_bg_opa(&um_switch_knob_checked_style, LV_OPA_COVER);
    lv_style_set_bg_color(&um_switch_knob_checked_style, bg4);
    // Border
    lv_style_set_border_opa(&um_switch_knob_checked_style, LV_OPA_COVER);
    lv_style_set_border_color(&um_switch_knob_checked_style, bg5);
    lv_style_set_border_width(&um_switch_knob_checked_style, 2);
    // Geometry
    lv_style_set_pad_ver(&um_switch_knob_checked_style, -knobPadding);
    lv_style_set_pad_hor(&um_switch_knob_checked_style, -knobPadding);
    lv_style_set_radius(&um_switch_knob_checked_style, knobRadius);

    lv_obj_remove_style(sw1, NULL, LV_STATE_FOCUS_KEY);
    lv_obj_remove_style(sw1, NULL, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_remove_style(sw1, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(sw1, NULL, LV_PART_KNOB);

//    lv_obj_remove_style_all(sw);        /*Remove the styles coming from the theme*/

    lv_obj_add_style(sw1, &um_switch_main_style, LV_PART_MAIN);
    lv_obj_add_style(sw1, &um_switch_main_style, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_add_style(sw1, &um_switch_main_style, LV_PART_MAIN | LV_STATE_PRESSED);


//    lv_obj_add_style(sw, &um_switch_indicator_style, LV_PART_INDICATOR);
    lv_obj_add_style(sw1, &um_switch_indicator_active_style, LV_PART_INDICATOR | LV_STATE_PRESSED);
    lv_obj_add_style(sw1, &um_switch_indicator_checked_style, LV_PART_INDICATOR | LV_STATE_CHECKED);
//
//    lv_obj_add_style(sw1, &um_switch_indicator_checked_style, LV_PART_INDICATOR);
//    lv_obj_add_style(sw1, &um_switch_indicator_checked_style, LV_PART_INDICATOR);
//    lv_obj_add_style(sw1, &um_switch_indicator_checked_style, LV_PART_INDICATOR | LV_STATE_HOVERED);

    lv_obj_add_style(sw1, &um_switch_knob_style, LV_PART_KNOB);
    lv_obj_add_style(sw1, &um_switch_knob_checked_style, LV_PART_KNOB | LV_STATE_CHECKED);
//    lv_obj_remove_style_all(sw);        /*Remove the styles coming from the theme*/





    lv_obj_add_state(sw1, LV_STATE_CHECKED);



//    lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);

}


void slider(lv_obj_t *parent) {
    /*Create a transition*/
    static const lv_style_prop_t props[] = {LV_STYLE_BG_COLOR, 0};
    static lv_style_transition_dsc_t transition_dsc;
    lv_style_transition_dsc_init(&transition_dsc, props, lv_anim_path_linear, 80, 0, NULL);

    static lv_style_t slider_dim_style;
    static lv_style_t style_indicator;
    static lv_style_t style_knob;
    static lv_style_t style_pressed_color;


    lv_style_init(&slider_dim_style);

    lv_style_set_bg_opa(&slider_dim_style, LV_OPA_COVER);
    lv_style_set_bg_color(&slider_dim_style, bg1);
    lv_style_set_border_color(&slider_dim_style, bg4);
    lv_style_set_radius(&slider_dim_style, 8);
    lv_style_set_height(&slider_dim_style, 48);
    lv_style_set_border_width(&slider_dim_style, 2);
    lv_style_set_pad_ver(&slider_dim_style, 4);
    lv_style_set_pad_hor(&slider_dim_style, 4);


    lv_style_init(&style_indicator);
    lv_style_set_transition(&style_indicator, &transition_dsc);
    lv_style_set_radius(&style_indicator, 4);
    lv_style_set_bg_color(&style_indicator, bg4);
    lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);

    lv_style_set_border_opa(&style_indicator, LV_OPA_0);


    lv_style_init(&style_knob);

    lv_style_set_bg_opa(&style_knob, LV_OPA_0);
    lv_style_set_border_opa(&style_knob, LV_OPA_0);

    lv_style_init(&style_pressed_color);
    lv_style_set_bg_color(&style_pressed_color, bg5);

    /*Create a slider and add the style*/
    lv_obj_t *slider = lv_slider_create(parent);
    lv_slider_set_mode(slider, LV_SLIDER_MODE_RANGE);
    lv_slider_set_range(slider, 0, 100);
    lv_obj_add_event_cb(slider, um_slider_button_cb, LV_EVENT_VALUE_CHANGED, slider);

    slider_label = lv_label_create(parent);
    lv_obj_set_align(slider_label, LV_ALIGN_RIGHT_MID);

    lv_obj_remove_style_all(slider);        /*Remove the styles coming from the theme*/

    lv_obj_add_style(slider, &slider_dim_style, LV_PART_MAIN);
    lv_obj_add_style(slider, &style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(slider, &style_pressed_color, LV_PART_INDICATOR | LV_STATE_PRESSED);
    lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);


    lv_obj_center(slider);
}

void um_udap_ui(lv_obj_t *parent) {

    lv_obj_t *screen = parent;

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    static int32_t row_dsc[] = {48, LV_GRID_FR(1), 86, LV_GRID_TEMPLATE_LAST};

    lv_obj_t *container = lv_obj_create(screen);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_center(container);
    lv_obj_add_style(container, &um_background_style, 0);
    lv_obj_set_grid_dsc_array(container, col_dsc, row_dsc);

    lv_obj_t *header = um_draw_header(container);
    lv_obj_set_grid_cell(header, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    um_body = lv_obj_create(container);
    lv_obj_add_style(um_body, &um_body_style, 0);
    lv_obj_set_grid_cell(um_body, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);


    lv_obj_t *dock = um_draw_dock(container);
    lv_obj_set_grid_cell(dock, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 2, 1);

    view_home = lv_obj_create(um_body);
    lv_obj_set_size(view_home, lv_pct(100), lv_pct(100));
    lv_obj_add_style(view_home, &um_container_style, 0);
    lv_obj_center(view_home);

    view_apps = lv_obj_create(um_body);
    lv_obj_set_size(view_apps, lv_pct(100), lv_pct(100));
    lv_obj_add_style(view_apps, &um_container_style, 0);
    lv_obj_center(view_apps);

    view_settings = lv_obj_create(um_body);
    lv_obj_set_flex_flow(view_settings, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(view_settings, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_size(view_settings, lv_pct(100), lv_pct(100));
    lv_obj_add_style(view_settings, &um_container_style, 0);
    lv_obj_center(view_settings);

    slider(view_settings);
    um_switch(view_settings, "Item 1", "Description");
    um_switch(view_settings, "Item 2", "Description");
    um_switch(view_settings, "Item 3", "Description");

    lv_obj_t *bellaImg = lv_image_create(view_home);
    lv_image_set_src(bellaImg, &bella_mini);
    lv_obj_align(bellaImg, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t *oliveImage = lv_image_create(view_apps);
    lv_image_set_src(oliveImage, &olive_mini);
    lv_obj_align(oliveImage, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t *label = lv_label_create(view_settings);
    lv_label_set_text(label, "UDAP");

    um_body_current = view_home;

    lv_obj_add_flag(view_apps, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(view_settings, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(view_home, LV_OBJ_FLAG_HIDDEN);

}
void ui_screen_main_init() {
    build_styles();
    um_udap_ui(lv_screen_active());
}