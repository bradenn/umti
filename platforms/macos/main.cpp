//
// Created by Braden Nicholson on 3/31/26.
//


#include "lvgl.h"
#include "hal_display.h"
#include "hal_input.h"

#include "screen_main.h"



int main(void) {
    lv_init();
    hal_display_init();
    hal_input_init();
    ui_screen_main_init();

    while (1) {

        lv_tick_inc(hal_display_tick());
        lv_timer_handler();
    }
}