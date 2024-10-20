#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

void gpu_r86_err_hand(reg86_t *r86);

int gpu_set_mode(uint16_t mode);

int gpu_init_mem_indexation(uint16_t mode);

int gpu_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

int aux_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

int aux_draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);

void aux_vg_commit();

int aux_clear();
