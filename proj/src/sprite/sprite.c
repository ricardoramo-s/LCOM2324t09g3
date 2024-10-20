#include "sprite.h"

Sprite *Sprite_create(xpm_map_t xpm, vec2 *offsets, vec2 *sizes, uint32_t framesc, uint32_t delay) {
  Sprite *sprite = (Sprite *) malloc(sizeof(Sprite));
  if (sprite == NULL) {
    return NULL; // memory allocation failed
  }

  Sprite_init(sprite, xpm, offsets, sizes, framesc, delay);
  return sprite;
}

void Sprite_init(Sprite *sprite, xpm_row_t *xpm, vec2 *offsets, vec2 *sizes, uint32_t framesc, uint32_t delay) {
  if (sprite == NULL) {
    return; // sprite is not allocated
  }

  sprite->img = (xpm_image_t *) malloc(sizeof(xpm_image_t));
  Sprite_xpm_load(sprite, xpm);

  sprite->offsets = (vec2 *) malloc(sizeof(vec2) * framesc);
  memcpy(sprite->offsets, offsets, sizeof(vec2) * framesc);

  sprite->sizes = (vec2 *) malloc(sizeof(vec2) * framesc);
  memcpy(sprite->sizes, sizes, sizeof(vec2) * framesc);
  
  sprite->framesc = framesc;
  sprite->current_frame = 0;
  sprite->current_delay = 0;
  sprite->delay = delay;
}

void Sprite_xpm_load(Sprite *sprite, xpm_row_t *xpm) {
  xpm_load(xpm, XPM_INDEXED, sprite->img);
}

void Sprite_draw(Sprite *sprite, vec2 position) {
  if (sprite->img == NULL) {
    printf("Null XPM!\n");
    return;
  }
  if (sprite->img->type == INVALID_XPM) {
    printf("Invalid XPM!\n");
    sprite->img = NULL;
    return;
  }

  if (sprite->transparent) {
    gpu_draw_transparent_xpm(sprite->img, position, sprite->offsets[sprite->current_frame], sprite->sizes[sprite->current_frame]);
  }
  else {
    gpu_draw_xpm(sprite->img, position, sprite->offsets[sprite->current_frame], sprite->sizes[sprite->current_frame]);
  }

  if (sprite->delay == 0) {
    return;
  }

  if (sprite->current_delay == sprite->delay) {
    sprite->current_delay = 0;
    sprite->current_frame += 1;
    sprite->current_frame = sprite->current_frame % sprite->framesc;
  }
  else {
    sprite->current_delay++;
  }

  return;
}
