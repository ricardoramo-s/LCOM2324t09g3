#include "assets_manager.h"


extern uint8_t vis_xsize;
extern uint8_t vis_ysize;
extern char visible_objects[MAX_YSIZE][MAX_XSIZE];

enum xpm_image_type type = XPM_INDEXED;

// char : #
struct BLOCK {
    xpm_image_t img;
    uint8_t *colors;
} block;

//char : p
struct PLAYER {
    xpm_image_t img;
    uint8_t *colors;
} player_sprite;


int assets_manager_init() {
    
    block.colors = xpm_load(block_xpm, type, &block.img);
    player_sprite.colors = xpm_load(player_xpm, type, &player_sprite.img);

    return 0;
}

int assets_manager_draw_img(xpm_image_t img,uint16_t posx,uint16_t posy, uint8_t *colors) {
    for (int iy = 0; iy < block.img.height; iy++) {
        for (int ix = 0; ix < block.img.width; ix++) {
            if (gpu_draw_pixel(posx + ix, posy + iy, *colors)) return 1;
            colors++;
        }
    }
    return 0;
}

int assets_manager_draw_asset(char asset, uint16_t posx, uint16_t posy) {


    switch (asset) {

        case '#': {
            uint8_t *colors = block.colors;
            if (assets_manager_draw_img(block.img, posx, posy, colors)) return 1;
            break;
        }

        case 'p': {
            uint8_t *colors = player_sprite.colors;
            if (assets_manager_draw_img(player_sprite.img, posx, posy, colors)) return 1;
        }

        default:
            break;
    }

    return 0;
}

int assets_manager_draw_player_vis() {


    for (uint8_t yi = vis_ysize - 1; yi >= 0; yi--) {
        for (uint8_t xi = 0; xi < vis_xsize; xi++) {
            if (assets_manager_draw_asset(visible_objects[yi][xi], xi * 64, yi * 64)) return 1;
        }
    }

    return 0;
}

