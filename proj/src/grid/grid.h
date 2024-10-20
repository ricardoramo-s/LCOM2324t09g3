#pragma once

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../drivers/gpu.h"

static struct GRID_INFO {
    uint8_t x_size;
    uint8_t y_size;
    char **objects;
} grid_info;

int grid_init(char *level);

void grid_cleanup();

struct GRID_INFO* grid_get_grid_info();
