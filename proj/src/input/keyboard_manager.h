#pragma once

#include "drivers/keyboard.h"

enum KB_KEY {
  KB_ESC,
  KB_UP,
  KB_DOWN,
  KB_LEFT,
  KB_RIGHT,
  KB_INVALID = 999,
};

void Keyboard_init();
void Keyboard_handle();
enum KB_KEY Keyboard_translate(uint8_t scancode, bool double_width, bool* make);
bool Keyboard_pressed(enum KB_KEY key);
