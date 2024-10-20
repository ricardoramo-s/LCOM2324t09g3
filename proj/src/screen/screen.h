#pragma once

#include <stdint.h>

#include "../drivers/gpu.h"
#include "../grid/grid.h"
#include "../proj_config.h"

static struct SCREEN_INFO {
    uint8_t x_size;
    uint8_t y_size;
    char **objects;
} screen_info;

int screen_init(double x, double y);

void screen_cleanup();

void screen_update();

struct SCREEN_INFO* screen_get_screen_info();

