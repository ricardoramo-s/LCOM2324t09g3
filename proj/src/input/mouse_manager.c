#include "mouse_manager.h"
#include "assets/sprites/mouse.h"
#include "drivers/gpu.h"
#include "drivers/mouse.h"
#include "gamestate/gamestate_manager.h"
#include "aabb/aabb.h"
#include "ui/ui_element.h"

struct packet mouse_packet;

static struct Mouse {
  vec2 position;
  Sprite* sprite;

  bool hidden;
  bool buttons[3];
  bool prev_buttons[3];
} mouse;

void Mouse_init() {
  mouse.position = VEC2(0, 0);
  mouse.hidden = false;

  memset(mouse.buttons, 0, sizeof(bool));
  memset(mouse.prev_buttons, 0, sizeof(bool));

  static vec2 offsets[] = {VEC2(0, 0), VEC2(34, 0)};
  static vec2 sizes[] = {VEC2(34, 42), VEC2(24, 38)};
  mouse.sprite = Sprite_create(mouse_xpm, offsets, sizes, 2, 0);
  mouse.sprite->current_frame = MOUSE_NORMAL;
  mouse.sprite->transparent = true;
}

void Mouse_handle() {
  mouse_read_packet();

  if (mouse_packet_ready()) {
    mouse_get_packet(&mouse_packet);
    // printf("LB: %d, MB: %d, RB: %d, Delta X: %d, Delta Y: %d\n", mouse_packet.lb, mouse_packet.mb, mouse_packet.rb, mouse_packet.delta_x, mouse_packet.delta_y);
    mouse.position.x += mouse_packet.delta_x;

    mouse.position.x = (mouse.position.x > 0) ? mouse.position.x : 0;
    mouse.position.x = (mouse.position.x < gpu_get_gpu_res_info()->x_res) ? mouse.position.x : gpu_get_gpu_res_info()->x_res - 1;

    mouse.position.y -= mouse_packet.delta_y;

    mouse.position.y = (mouse.position.y > 0) ? mouse.position.y : 0;
    mouse.position.y = (mouse.position.y < gpu_get_gpu_res_info()->y_res) ? mouse.position.y : gpu_get_gpu_res_info()->y_res - 1;

    mouse.prev_buttons[MOUSE_LB] = mouse.buttons[MOUSE_LB];
    mouse.prev_buttons[MOUSE_MB] = mouse.buttons[MOUSE_MB];
    mouse.prev_buttons[MOUSE_RB] = mouse.buttons[MOUSE_RB];

    mouse.buttons[MOUSE_LB] = mouse_packet.lb;
    mouse.buttons[MOUSE_MB] = mouse_packet.mb;
    mouse.buttons[MOUSE_RB] = mouse_packet.rb;

    printf("Mouse Position: (%d,%d)\n", mouse.position.x, mouse.position.y);
  }
}

void Mouse_draw() {
  // TEMPORARY
  if (Gamestate_get_state() == GS_MAIN || Gamestate_get_state() == GS_LEADERBOARD) {
    Menu* menu = Gamestate_get_current_menu();
    Node* node = menu->element;
    Mouse_state(MOUSE_NORMAL);

    while (node != NULL) {
      ui_element* element = (ui_element *) node->data;
      if (AABB_contains(&element->aabb, &mouse.position) && element->on_click != NULL) {
        if (mouse.buttons[MOUSE_LB] && mouse.prev_buttons[MOUSE_LB] != mouse.buttons[MOUSE_LB]) {
          element->on_click();
        }
        
        Mouse_state(MOUSE_CLICK);
        break;
      }

      node = node->next;
    }
  }

  if (!mouse.hidden) {
    if (mouse.sprite->current_frame == MOUSE_CLICK) {
      Sprite_draw(mouse.sprite, VEC2(mouse.position.x - 9, mouse.position.y)); // workaround to offset the tip of the cursor
    }
    else {
      Sprite_draw(mouse.sprite, mouse.position);
    }
  }
}

void Mouse_show() {
  mouse.hidden = false;
}

void Mouse_hide() {
  mouse.hidden = true;
}

void Mouse_reset() {
  mouse.position = VEC2(0, 0);
}

void Mouse_state(enum MOUSE_STATE state) {
  mouse.sprite->current_frame = state;
}

vec2 Mouse_position() {
  return mouse.position;
}

bool Mouse_button(enum MOUSE_BUTTON button) {
  return mouse.buttons[button];
}
