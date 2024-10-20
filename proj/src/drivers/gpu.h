#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <vec2/vec2.h>
#include <stdbool.h>

static struct GPU_RES_INFO {
    uint16_t x_res;
    uint16_t y_res;
} gpu_res_info = {0, 0};

int gpu_set_mode(uint16_t mode);

int gpu_init_mem_indexation(uint16_t mode);

int gpu_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

int gpu_draw_line(vec2 position, uint8_t *bytes, int pixelsc);

int gpu_draw_rect(vec2 position, vec2 size, uint8_t color);

int gpu_draw_transparent_xpm(xpm_image_t *xpm, vec2 position, vec2 offset, vec2 size);

int gpu_draw_xpm(xpm_image_t* xpm, vec2 position, vec2 offset, vec2 size);

int aux_draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);

void gpu_buffer_commit();

void gpu_buffer_clear();

int aux_clear();

struct GPU_RES_INFO* gpu_get_gpu_res_info();
