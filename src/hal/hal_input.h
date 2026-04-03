//
// Created by Braden Nicholson on 3/31/26.
//

#ifndef UDAP_MICRO_UI_HAL_INPUT_H
#define UDAP_MICRO_UI_HAL_INPUT_H

#pragma once
#include "lvgl.h"
void hal_input_init(void);
void hal_input_read(lv_indev_t * indev, lv_indev_data_t * data);

#endif //UDAP_MICRO_UI_HAL_INPUT_H
