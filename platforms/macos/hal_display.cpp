//
// Created by Braden Nicholson on 3/31/26.
//
#include "lvgl.h"
#include "hal_display.h"
#include <SDL2/SDL.h>

#define DISPLAY_W 600
#define DISPLAY_H 1024

static SDL_Window   *window;
static SDL_Renderer *renderer;
static SDL_Texture  *texture;

static void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    SDL_UpdateTexture(texture, NULL, px_map, DISPLAY_W * 4);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    lv_display_flush_ready(disp);
}

static uint32_t lastTick = 0;

uint32_t hal_display_tick(void) {
    SDL_PumpEvents();

    lastTick = SDL_GetTicks();
    SDL_Delay(5);
    Uint32 current = SDL_GetTicks();
    uint32_t delta = current- lastTick;
    // Update the tick timer. Tick is new for LVGL 9
    lastTick = current;
    return delta;
}

void hal_display_init(void) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    window   = SDL_CreateWindow("UI Preview", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, DISPLAY_W, DISPLAY_H, SDL_WINDOW_ALLOW_HIGHDPI);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture  = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_STREAMING, DISPLAY_W, DISPLAY_H);

    lv_display_t *disp = lv_display_create(DISPLAY_W, DISPLAY_H);
    lv_display_set_flush_cb(disp, flush_cb);

    static uint8_t buf[DISPLAY_W * DISPLAY_H * 4];
    lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_FULL);
}