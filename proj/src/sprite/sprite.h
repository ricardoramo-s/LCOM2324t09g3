#pragma once

#include <drivers/gpu.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vec2/vec2.h>

typedef struct {
  xpm_image_t *img;

  vec2 *offsets;    // relative position inside the img
  vec2 *sizes;      // size of frame inside the img
  uint32_t framesc; // number of frames

  uint32_t delay; // the n of interrupts between frames

  uint32_t current_frame; // the current frame of the sprite
  uint32_t current_delay;

  bool transparent; // set if it need to be transparent (index 99)

} Sprite;

Sprite *Sprite_create(xpm_row_t *xpm, vec2 *offsets, vec2 *sizes, uint32_t framesc, uint32_t delay);

void Sprite_init(Sprite *sprite, xpm_row_t *xpm, vec2 *offsets, vec2 *sizes, uint32_t framesc, uint32_t delay);

void Sprite_xpm_load(Sprite *sprite, xpm_row_t *xpm);

void Sprite_draw(Sprite *sprite, vec2 position);
