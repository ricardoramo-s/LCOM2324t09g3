#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "gpu.h"
#include "../proj/src/drivers/i8042.h"
#include "../proj/src/drivers/keyboard.h"
#include "../proj/src/drivers/timer.h"


extern uint8_t scan_code;
extern bool kbd_ih_valid;
extern size_t counter;

extern uint8_t color_mode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int aux_wait_esc() {

    uint8_t bit_no;

    if (keyboard_subscribe(&bit_no)) return 1;

    int r, ipc_status;
    message msg;

    while (scan_code != ESC_BREAKCODE) {

        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {

            switch (_ENDPOINT_P(msg.m_source)) {

                case HARDWARE:

                    if (msg.m_notify.interrupts & bit_no) kbc_ih();
                    break;

                default:
                    break;
            }
        }
        else {
        }
    }

    if (keyboard_unsubscribe()) return 1;
    return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  
    if (gpu_set_mode(mode)) return 1;

    sleep(delay);

    if (vg_exit()) return 1;
 
    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
 

    if (gpu_init_mem_indexation(mode)) return 1;

    if (gpu_set_mode(mode)) return 1;

    if (vg_draw_rectangle(x, y, width, height, color)) return 1;

    if (aux_wait_esc()) return 1;

    if (vg_exit()) return 1;

    return 0;
}


int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

    if (gpu_init_mem_indexation(mode)) return 1;

    if (gpu_set_mode(mode)) return 1;

    if (aux_draw_pattern(no_rectangles, first, step)) return 1;
    aux_vg_commit();

    if (aux_wait_esc()) return 1;

    if (vg_exit()) return 1;

    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
    if (gpu_init_mem_indexation(0x105)) return 1;

    if (gpu_set_mode(0x105)) return 1;

    aux_draw_xpm(xpm, x, y);

    aux_vg_commit();

    if (aux_wait_esc()) return 1;

    if (vg_exit()) return 1;

    return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  

    uint8_t kbd_bit_no;
    uint8_t timer_bit_no;

    if (keyboard_subscribe(&kbd_bit_no)) return 1;
    if (timer_subscribe(&timer_bit_no)) return 1;

    uint16_t frequency = sys_hz();
    uint16_t clocks_per_frame = frequency / fr_rate;

    uint16_t x = xi;
    uint16_t y = yi;

    int r, ipc_status;
    message msg;

    if (gpu_set_mode(0x105)) return 1;
    if (gpu_init_mem_indexation(0x105)) return 1;


    while (scan_code != ESC_BREAKCODE) {

        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {

            switch (_ENDPOINT_P(msg.m_source)) {

                case HARDWARE:

                    if (msg.m_notify.interrupts & kbd_bit_no) {
                        kbc_ih();
                    }

                    if (msg.m_notify.interrupts & timer_bit_no) {
                        timer_int_handler();
                        if (counter % clocks_per_frame == 0) {
                            x++;
                            y++;
                            if (aux_clear()) return 1;
                            if (aux_draw_xpm(xpm, x, y)) return 1;
                            aux_vg_commit();
                        }
                    }

                    break;


                default:
                    break;
            }
        }
        else {
        }
    }

    if (vg_exit()) return 1;

    if (keyboard_unsubscribe()) return 1;
    if (timer_unsubscribe()) return 1;

    return 0;

}

int(video_test_controller)() {
    
    uint8_t kbd_bit_no;
    uint8_t timer_bit_no;

    if (keyboard_subscribe(&kbd_bit_no)) return 1;
    if (timer_subscribe(&timer_bit_no)) return 1;

    if (gpu_set_mode(0x105)) return 1;
    if (gpu_init_mem_indexation(0x105)) return 1;

    uint8_t fr_rate = 30;

    uint16_t frequency = sys_hz();
    uint16_t clocks_per_frame = frequency / fr_rate;

    uint16_t x = 100;
    uint16_t y = 100;

    int r, ipc_status;
    message msg;

    int cooldown = 1 * frequency;
    int elapsed = cooldown;

    enum moveState {
        RUNNING = 10,
        WALKING = 1
    };

    enum keyState {
        PRESSED = 1,
        NOT_PRESED = 0
    };

    enum moveState move_state = WALKING;

    enum keyState w_key_state = NOT_PRESED;
    enum keyState a_key_state = NOT_PRESED;
    enum keyState s_key_state = NOT_PRESED;
    enum keyState d_key_state = NOT_PRESED;


    while (scan_code != ESC_BREAKCODE) {

        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d", r);
            continue;
        }

        if (!elapsed) {

            if (w_key_state) {
                y -= 10;
                elapsed = cooldown / move_state;
            }

            if (a_key_state) {
                x -= 10;
                elapsed = cooldown / move_state;
            }

            if (s_key_state) {
                y += 10;
                elapsed = cooldown / move_state;
            }

            if (d_key_state) {
                x += 10;
                elapsed = cooldown / move_state;
            }
        }
        

        if (is_ipc_notify(ipc_status)) {

            switch (_ENDPOINT_P(msg.m_source)) {

                case HARDWARE:

                    if (msg.m_notify.interrupts & kbd_bit_no) {
                        kbc_ih();
                        if (kbd_ih_valid) {

                            if (scan_code == SHIFT_MAKECODE) move_state = RUNNING;
                            if (scan_code == SHIFT_BREAKCODE) move_state = WALKING;

                            if (scan_code == W_MAKECODE) w_key_state = PRESSED;
                            if (scan_code == W_BREAKCODE) w_key_state = NOT_PRESED;

                            if (scan_code == A_MAKECODE) a_key_state = PRESSED;
                            if (scan_code == A_BREAKCODE) a_key_state = NOT_PRESED;

                            if (scan_code == S_MAKECODE) s_key_state = PRESSED;
                            if (scan_code == S_BREAKCODE) s_key_state = NOT_PRESED;

                            if (scan_code == D_MAKECODE) d_key_state = PRESSED;
                            if (scan_code == D_BREAKCODE) d_key_state = NOT_PRESED;

                        }
                    }

                    if (msg.m_notify.interrupts & timer_bit_no) {
                        timer_int_handler();

                        if (elapsed) elapsed--;

                        if (counter % clocks_per_frame == 0) {
                            aux_clear();
                            if (aux_draw_xpm(penguin, x, y)) return 1;
                            aux_vg_commit();
                        }
                    }

                    break;

                default:
                    break;
            }
        }
        else {
        }
    }

    if (vg_exit()) return 1;

    if (keyboard_unsubscribe()) return 1;
    if (timer_unsubscribe()) return 1;
  
    return 0;
}   
