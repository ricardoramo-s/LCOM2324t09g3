// alphabet_sprites.h
#pragma once

#include <stdint.h>

#include "alphabet_sprites.h"

typedef struct {
    int x, y, width, height;
} CharSprite;

#define CharSprite(x, y, width, height) ((CharSprite){x, y, width, height})

static int CHAR_HEIGHT = 83;
static int CHAR_WIDTH = 44;

CharSprite calculate_char_coord(char c) {
    int index;
    if (c == ',') {
        index = 0;
    } else if (c == '-') {
        index = 1;
    } else if (c == '.') {
        index = 2;
    } else if (c == '/') {
        index = 3;
    } else if (c >= '0' && c <= '9') {
        index = 4 + (c - '0');
    } else if (c == ':') {
        index = 14;
    } else if (c >= 'A' && c <= 'Z') {
        index = 15 + (c - 'A');
    } else if (c == '|') {
        index = 41;
    } else {
        return CharSprite(0, 0, 0, 0);
    }

    CharSprite coord;
    coord.x = 0; 
    coord.y = index * CHAR_HEIGHT;
    coord.width = CHAR_WIDTH;
    coord.height = CHAR_HEIGHT;

    return coord;
}
