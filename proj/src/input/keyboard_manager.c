#include "keyboard_manager.h"

bool double_width;
static bool keys[20];

void Keyboard_init() {
  memset(keys, 0, sizeof(bool));
  double_width = false;
}

void Keyboard_handle() {
  uint8_t scancode;
  keyboard_read_scancode(&scancode);

  if (scancode == 0xe0 && !double_width) {
    double_width = true;
  }
  else {
    bool make;
    enum KB_KEY key = Keyboard_translate(scancode, double_width, &make);

    if (key != KB_INVALID) {
      printf("changed keys[%d] to %d\n", key, make);
      keys[key] = (make) ? true : false;
    }

    double_width = false;
    printf("Scancode: %d  Make: %d\n", scancode, make);
  }
}

enum KB_KEY Keyboard_translate(uint8_t scancode, bool double_width, bool *make) {
  *make = (scancode & 0x80) ? false : true;

  if (!*make) {
    scancode &= 0x7f;
  }

  //printf("translate scancode: %d\n", scancode);

  if (double_width) {
    switch (scancode) {
      case 0x48:
        return KB_UP;
      case 0x4B:
        return KB_LEFT;
      case 0x50:
        return KB_DOWN;
      case 0x4D:
        return KB_RIGHT;

      default:
        return KB_INVALID;
    }
  }
  else {
    switch (scancode) {
      case 0x01:
        return KB_ESC;

      default:
        return KB_INVALID;
    }
  }
}

bool Keyboard_pressed(enum KB_KEY key) {
  return keys[key];
}
