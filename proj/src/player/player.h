#pragma once

#include <stdint.h>
#include <stdio.h>

#include "../grid/grid.h"

#define MAX_XSIZE 18
#define MAX_YSIZE 14

int player_init(uint8_t x, uint8_t y);

void player_reset_visible();

void player_set_visible();

