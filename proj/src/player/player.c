#include "player.h"

const uint8_t vis_xsize = 18;
const uint8_t vis_ysize = 14;
char visible_objects[MAX_YSIZE][MAX_XSIZE];

const int midx = vis_xsize / 2;
const int midy = vis_ysize / 2;
const int minx = 0 - midx;
const int miny = 0 - midy;
const int maxx = 0 + midx;
const int maxy = 0 + midy;

uint8_t player_posx;
uint8_t player_posy;

extern char **grid_info_objects;
extern uint8_t grid_sizex;
extern uint8_t grid_sizey;

int player_init(uint8_t x, uint8_t y) {

	if (x > grid_sizex || y > grid_sizey) {
        printf("Invalid location for player");
        return 1;
	}

    player_posx = x;
    player_posy = y;

	player_set_visible();
    visible_objects[midy][midx] = 'p';

    return 1;
}

void player_reset_visible() {

	for (int yi = 0; yi < vis_ysize; yi++) {
        for (int xi = 0; xi < vis_xsize; xi++) {
            visible_objects[yi][xi] = ' ';
        }
    }
}

void player_set_visible() {

    player_reset_visible();

    
    for (int yi = miny; yi < maxy; yi++) {
        printf("%d", player_posy + yi < 0 || player_posy + yi >= grid_sizey);
        if (player_posy + yi < 0 || player_posy + yi >= grid_sizey) continue;
        for (int xi = minx; xi < maxx; xi++) {
            printf("%d", player_posy + xi < 0 || player_posy + xi >= grid_sizex);
            if (player_posy + xi < 0 || player_posy + xi >= grid_sizex) continue;
            
            visible_objects[yi + maxy][xi + maxx] = grid_info_objects[player_posy + yi][player_posx + xi];
        }
    }

    
    /*
    for (int yi = 0; yi < vis_ysize; yi++) {
        for (int xi = 0; xi < vis_xsize; xi++) {
            visible_objects[yi][xi] = grid_objects[yi][xi];
        }
    }
    */
    
}

