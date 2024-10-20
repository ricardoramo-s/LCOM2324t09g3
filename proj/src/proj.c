
/*
 * mode 105h
 * xres 1024
 * yres 768
 *
 * grid xres 64
 * grid yres 64
 *
 * grid xsize 16
 * grid ysize 12
 */

#include "assets/assets_manager.h"
#include "grid/grid.h"
#include "player/player.h"
#include "screen/screen.h"
#include "sprite/sprite.h"
#include "vec2/vec2.h"
#include "input/mouse_manager.h"
#include "input/keyboard_manager.h"
#include "ui/menu.h"
#include "gamestate/gamestate_manager.h"

#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "drivers/i8042.h"
#include "drivers/i8254.h"

#include "drivers/gpu.h"
#include "drivers/kbc.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/timer.h"

#include "assets/sprites/block.h"
#include "assets/sprites/mario_run.h"
#include "assets/sprites/mouse.h"
#include "assets/sprites/logo.h"
#include "assets/sprites/button.h"

#include "proj_config.h"

#define TRANSPARENCY_COLOR_INDEXED 6

uint8_t timer_bit_no;
uint8_t kbc_bit_no;
uint8_t mouse_bit_no;

uint8_t fps = 30;

int main(int argc, char *argv[]) {

  lcf_set_language("EN-US");

  lcf_trace_calls("/home/lcom/labs/proj/src/Errors/trace.txt");

  lcf_log_output("/home/lcom/labs/proj/src/Errors/output.txt");

  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();

  return 0;
}

int proj_init() {
  if (keyboard_subscribe(&kbc_bit_no)) return 1;
  if (timer_subscribe(&timer_bit_no)) return 1;
  if (mouse_subscribe(&mouse_bit_no)) return 1;

  if (mouse_write_command(0xF3)) return 1;
  if (mouse_write_command(40)) return 1;

  if (mouse_enable_data()) return 1;

  if (gpu_set_mode(GRAPHICS_MODE)) return 1;
  if (gpu_init_mem_indexation(GRAPHICS_MODE)) return 1;

  Mouse_init();
  Keyboard_init();
  Gamestate_init();

  return 0;
}

int proj_shutdown() {
  if (mouse_disable_data()) return 1;

  if (keyboard_unsubscribe()) return 1;
  if (timer_unsubscribe()) return 1;
  if (mouse_unsubscribe()) return 1;

  if (vg_exit()) return 1;
  return 0;
}

int(proj_main_loop)(int argc, char **argv) {

  if (proj_init()) return 1;

  int ipc_status, r;
  message msg;
  bool skip_frame = false;

  while (Gamestate_get_state() != GS_QUIT) {
    // Get a request message.
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                  // hardware interrupt notification
          if (msg.m_notify.interrupts & mouse_bit_no) { // mouse
            Mouse_handle();
          }
          if (msg.m_notify.interrupts & kbc_bit_no) { // keyboard
            Keyboard_handle();

            // TEMPORARY
            if ((Gamestate_get_state() == GS_MAIN || Gamestate_get_state() == GS_LEADERBOARD) && Keyboard_pressed(KB_ESC)) {
              Gamestate_quit();
            }
          }
          if (msg.m_notify.interrupts & timer_bit_no) { // timer
            if (!skip_frame) {
              if (Gamestate_get_state() == GS_MAIN || Gamestate_get_state() == GS_LEADERBOARD) { // menu
                Menu_draw(Gamestate_get_current_menu());
              }
              
              Mouse_draw();
              gpu_buffer_commit();
              gpu_buffer_clear();
            }

            skip_frame = !skip_frame;
          }
          break;
        default:
          break; // no other notifications expected do nothing
      }
    }
    else { // received a standard message, not a notification
           // no standard messages expected do nothing
    }
  }

  if (proj_shutdown()) return 1;
  return 0;
}


/**
 
 Leaderboard leaderboard;
    struct rtc_date_time rdt;
    rtc_read_date_time(&rdt);
    leaderboard.entry_count = 0;
    strcpy(leaderboard.entries[0].name, "AAA");
    leaderboard.entries[0].dt_of_entry = rdt;
    leaderboard.entries[0].time_entry = 100;
    leaderboard.entry_count++;
    
    strcpy(leaderboard.entries[1].name, "BBB");
    leaderboard.entries[1].dt_of_entry = rdt;
    leaderboard.entries[1].time_entry = 120;
    leaderboard.entry_count++;

    strcpy(leaderboard.entries[2].name, "CCC");
    leaderboard.entries[2].dt_of_entry = rdt;
    leaderboard.entries[2].time_entry = 200;
    leaderboard.entry_count++;

    if (Leaderboard_read_file(&leaderboard) != 0) {
        printf("Leaderboard_write_file: FAIL\n");
        return 1;
    }

    Leaderboard_add_entry(&leaderboard, "TIA", rdt, 10);
    if (Leaderboard_write_file(&leaderboard) != 0) {
        printf("Leaderboard_write_file: FAIL\n");
        return 1;
    }
    return 0;
**/
