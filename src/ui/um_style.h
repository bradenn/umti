//
// Created by Braden Nicholson on 3/27/26.
//
#include "lvgl.h"

#ifndef UMTI_UM_STYLE_H
#define UMTI_UM_STYLE_H

const lv_color_t bg0 = lv_color_hex(0x0f0f10);
const lv_color_t bg1 = lv_color_hex(0x161618);
const lv_color_t bg2 = lv_color_hex(0x1c1c1f);
const lv_color_t bg3 = lv_color_hex(0x232326);
const lv_color_t bg4 = lv_color_hex(0x2b2b2f);
const lv_color_t bg5 = lv_color_hex(0x34343a);
const lv_color_t bg6 = lv_color_hex(0x3f3f46);
const lv_color_t bg7 = lv_color_hex(0x4a4a52);
const lv_color_t bg8 = lv_color_hex(0x5a5a63);

const lv_color_t fg0 = lv_color_hex(0xf2f2f3);
const lv_color_t fg1 = lv_color_hex(0xd6d6d8);
const lv_color_t fg2 = lv_color_hex(0xb8b8bc);
const lv_color_t fg3 = lv_color_hex(0x9a9aa0);
const lv_color_t fg4 = lv_color_hex(0x7e7e85);
const lv_color_t fg5 = lv_color_hex(0x66666d);
const lv_color_t fg6 = lv_color_hex(0x4f4f56);

LV_IMAGE_DECLARE(home_icon);
LV_IMAGE_DECLARE(apps_icon);
LV_IMAGE_DECLARE(settings_icon);
LV_IMAGE_DECLARE(wifi_1_icon);
LV_IMAGE_DECLARE(wifi_2_icon);
LV_IMAGE_DECLARE(wifi_3_icon);

LV_IMAGE_DECLARE(bella_mini);
LV_IMAGE_DECLARE(olive_mini);

LV_FONT_DECLARE(roboto_600);
LV_FONT_DECLARE(roboto_mono_18);
LV_FONT_DECLARE(roboto_black_18);


static lv_style_t um_background_style;
static lv_style_t um_container_style;
static lv_style_t um_body_style;
static lv_style_t um_dock_style;
static lv_style_t um_button_style;
static lv_style_t um_button_active_style;
static lv_style_t um_header_style;
static lv_style_t um_slider_dim_style;


static void build_styles() {

    // Background Style

    lv_style_init(&um_background_style);
    lv_style_set_bg_color(&um_background_style, bg0);
    lv_style_set_text_font(&um_background_style, &lv_font_montserrat_18);
    lv_style_set_text_color(&um_background_style, fg0);
    lv_style_set_pad_all(&um_background_style, 16);
    lv_style_set_radius(&um_background_style, 0);
    lv_style_set_border_side(&um_background_style, LV_BORDER_SIDE_NONE);

    // Background Style

    lv_style_init(&um_container_style);
    lv_style_set_bg_opa(&um_container_style, LV_OPA_0);
    lv_style_set_text_font(&um_container_style, &lv_font_montserrat_18);
    lv_style_set_text_color(&um_container_style, fg0);
    lv_style_set_pad_all(&um_container_style, 0);
    lv_style_set_radius(&um_container_style, 0);
    lv_style_set_border_side(&um_container_style, LV_BORDER_SIDE_NONE);


    // Body Style

    lv_style_init(&um_body_style);
    lv_style_set_bg_color(&um_body_style, bg2);
    lv_style_set_text_font(&um_body_style, &lv_font_montserrat_18);
    lv_style_set_text_color(&um_body_style, fg0);
    lv_style_set_outline_pad(&um_body_style, 8);
    lv_style_set_pad_all(&um_body_style, 8);
    lv_style_set_radius(&um_body_style, 16);
    lv_style_set_border_color(&um_body_style, bg3);
    lv_style_set_border_side(&um_body_style, LV_BORDER_SIDE_FULL);

    // Dock Style

    lv_style_init(&um_dock_style);
    lv_style_set_bg_color(&um_dock_style, bg2);
    lv_style_set_text_font(&um_dock_style, &lv_font_montserrat_18);
    lv_style_set_text_color(&um_dock_style, fg0);
    lv_style_set_outline_pad(&um_dock_style, 8);
    lv_style_set_pad_all(&um_dock_style, 8);
    lv_style_set_radius(&um_dock_style, 16);
    lv_style_set_border_color(&um_dock_style, bg3);
    lv_style_set_border_side(&um_dock_style, LV_BORDER_SIDE_FULL);

    // Header Style

    lv_style_init(&um_header_style);
    lv_style_set_text_font(&um_header_style, &roboto_mono_18);
    lv_style_set_text_color(&um_header_style, fg2);
    lv_style_set_text_align(&um_header_style, LV_TEXT_ALIGN_RIGHT);

    // Button Style

    lv_style_init(&um_button_style);
    lv_style_set_bg_color(&um_button_style, bg3);
    lv_style_set_text_font(&um_button_style, &lv_font_montserrat_18);
    lv_style_set_text_color(&um_button_style, fg0);
    lv_style_set_radius(&um_button_style, 8);
    lv_style_set_outline_pad(&um_button_style, 8);
    lv_style_set_border_width(&um_button_style, 2);
    lv_style_set_border_color(&um_button_style, bg4);
    lv_style_set_border_side(&um_button_style, LV_BORDER_SIDE_FULL);

    lv_style_set_shadow_offset_x(&um_button_style, 0);
    lv_style_set_shadow_offset_y(&um_button_style, 0);
    lv_style_set_shadow_opa(&um_button_style, LV_OPA_0);

    lv_style_set_shadow_spread(&um_button_style, 0);
    lv_style_set_shadow_width(&um_button_style, 0);
    lv_style_set_shadow_color(&um_button_style, bg0);

    // Button Style Active

    lv_style_init(&um_button_active_style);

    static const lv_color_t grad_colors[2] = {
            bg2,
            bg5,
    };

    static const lv_opa_t grad_opa[2] = {
            LV_OPA_100,
            LV_OPA_100,
    };

    static lv_grad_dsc_t grad;
    lv_gradient_init_stops(&grad, grad_colors, grad_opa, NULL, sizeof(grad_colors) / sizeof(lv_color_t));
    lv_grad_linear_init(&grad, lv_pct(0), lv_pct(85), lv_pct(100), lv_pct(15), LV_GRAD_EXTEND_PAD);

    lv_style_set_bg_grad(&um_button_active_style, &grad);
    lv_style_set_text_font(&um_button_active_style, &lv_font_montserrat_18);
    lv_style_set_text_color(&um_button_active_style, fg0);
    lv_style_set_radius(&um_button_active_style, 8);
    lv_style_set_outline_pad(&um_button_active_style, 8);
    lv_style_set_border_width(&um_button_active_style, 2);
    lv_style_set_border_color(&um_button_active_style, bg4);
    lv_style_set_border_side(&um_button_active_style, LV_BORDER_SIDE_FULL);
    lv_style_set_shadow_offset_x(&um_button_active_style, 0);
    lv_style_set_shadow_offset_y(&um_button_active_style, 0);



}

enum {
    ICON_NONE = 0,
    ICON_HOME = 1,
    ICON_APPS = 2,
    ICON_SETTINGS = 3,
} typedef um_icon_t;
extern void um_boot_set_state(const char * state, int progress);
extern lv_obj_t* um_boot();


#endif //UMTI_UM_STYLE_H
