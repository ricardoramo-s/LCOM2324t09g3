#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "../proj/src/drivers/i8042.h"
#include "../proj/src/drivers/keyboard.h"
#include "../proj/src/drivers/timer.h"


extern uint8_t scan_code;
extern bool kbd_ih_valid;
extern size_t counter;
//extern uint16_t inb_count;

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab3/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
    return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(kbd_test_scan)() {

    uint8_t bit_no;

    if (keyboard_subscribe(&bit_no)) return 1;

    uint8_t bytes[2] = {0};
    int r, ipc_status;
    message msg;

    while (scan_code != ESC_BREAKCODE) {

        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d", r);
            continue;
        }


        if (is_ipc_notify(ipc_status)) { /* received notification */
            
            switch (_ENDPOINT_P(msg.m_source)) {
              
            case HARDWARE: /* hardware interrupt notification */
          
                if (msg.m_notify.interrupts & bit_no) { /* subscribed interrupt */

                    kbd_ih_valid = 0;
                    kbc_ih();

                    if (kbd_ih_valid) {

                        if (scan_code == DOUBLE_BYTE) {
                            bytes[0] = scan_code;
                            continue;
                        }

                        if (bytes[0] == DOUBLE_BYTE) {
                            bytes[1] = scan_code;
                            kbd_print_scancode(!(scan_code & BREAK_BIT), 2, bytes);
                            bytes[0] = 0x00;
                            continue;
                        }

                        bytes[0] = scan_code;
                        kbd_print_scancode(!(scan_code & BREAK_BIT), 1, bytes);
                    }

                }
                break;

            default:
                break; /* no other notifications expected do nothing */
            }
        }
        else { /* received a standard message, not a notification */
               /* no standard messages expected do nothing */
        }
    }

    if (keyboard_unsubscribe()) return 1;

    return 0;
}

int(kbd_test_poll)() {

    uint8_t bytes[2] = {0};
    
    do {
        kbd_ih_valid = 0;
        kbc_ih();

        if (kbd_ih_valid) {

            if (scan_code == DOUBLE_BYTE) {
                bytes[0] = scan_code;
                continue;
            }

            if (bytes[0] == DOUBLE_BYTE) {
                bytes[1] = scan_code;
                kbd_print_scancode(!(scan_code & BREAK_BIT), 2, bytes);
                bytes[0] = 0x00;
                continue;
            }

            bytes[0] = scan_code;
            kbd_print_scancode(!(scan_code & BREAK_BIT), 1, bytes);
        }

    } while (scan_code != ESC_BREAKCODE);

    if (keyboard_enable_interrupts()) return 1;
    
    return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {

    uint8_t timer_bit_no, kbd_bit_no, count = n;

    if (keyboard_subscribe(&kbd_bit_no)) return 1;
   
    if (timer_subscribe(&timer_bit_no)) return 1;

    uint16_t timer_freq = sys_hz();
    uint8_t elapsed = 0;

    uint8_t bytes[2] = {0};
    int r, ipc_status;
    message msg;

    while ((scan_code != ESC_BREAKCODE) && (elapsed < n)) {

        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) { 

            switch (_ENDPOINT_P(msg.m_source)) {

                case HARDWARE:

                    if (msg.m_notify.interrupts & kbd_bit_no) {

                        elapsed = 0;
                        kbd_ih_valid = 0;
                        kbc_ih();

                        if (kbd_ih_valid) {
                            count = n;

                            if (scan_code == DOUBLE_BYTE) {
                                bytes[0] = scan_code;
                                continue;
                            }

                            if (bytes[0] == DOUBLE_BYTE) {
                                bytes[1] = scan_code;
                                kbd_print_scancode(!(scan_code & BREAK_BIT), 2, bytes);
                                bytes[0] = 0x00;
                                continue;
                            }

                            bytes[0] = scan_code;
                            kbd_print_scancode(!(scan_code & BREAK_BIT), 1, bytes);
                        }
                    }
                        

                    if (msg.m_notify.interrupts & timer_bit_no) { 
                        timer_int_handler();
                        if (counter % timer_freq == 0) elapsed++;
                    }

                    break;

                default:
                    break; 
            }
        }
        else { 
               
        }
    }

    if (keyboard_unsubscribe()) return 1;
    if (timer_unsubscribe()) return 1;


    return 0;
}
