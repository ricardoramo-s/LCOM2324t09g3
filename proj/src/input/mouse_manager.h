#pragma once

#include "vec2/vec2.h"
#include "sprite/sprite.h"
#include "stdbool.h"

enum MOUSE_STATE {
  MOUSE_CLICK = 0,
  MOUSE_NORMAL = 1,
};

enum MOUSE_BUTTON {
  MOUSE_LB = 0, 
  MOUSE_MB = 1, 
  MOUSE_RB = 2,
};

void Mouse_init();
void Mouse_handle();
void Mouse_draw();
void Mouse_show();
void Mouse_hide();
void Mouse_reset();
void Mouse_state(enum MOUSE_STATE state);
vec2 Mouse_position();
bool Mouse_button(enum MOUSE_BUTTON button);
