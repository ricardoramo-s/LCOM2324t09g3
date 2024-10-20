#pragma once

#include <stdint.h>
#include <lcom/lcf.h>

#include "../drivers/gpu.h"
#include "sprites/block.h"
#include "sprites/player_sprite.h"
#include "../player/player.h"

int assets_manager_init();

int assets_manager_draw_asset(char asset, uint16_t posx, uint16_t posy);

int assets_manager_draw_img(xpm_image_t img, uint16_t posx, uint16_t posy, uint8_t *colors);

int assets_manager_draw_player_vis();

