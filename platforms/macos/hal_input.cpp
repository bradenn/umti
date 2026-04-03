#include "lvgl.h"
#include "hal_input.h"
#include <SDL2/SDL.h>

static int32_t last_x, last_y;
static bool pressed = false;

void hal_input_init(void) {
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, hal_input_read);
}

void hal_input_read(lv_indev_t * indev, lv_indev_data_t * data) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);
        if (e.type == SDL_MOUSEBUTTONDOWN) pressed = true;
        if (e.type == SDL_MOUSEBUTTONUP)   pressed = false;
        if (e.type == SDL_MOUSEMOTION) {
            last_x = e.motion.x;
            last_y = e.motion.y;
        }
    }
    data->point.x = last_x;
    data->point.y = last_y;
    data->state   = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}