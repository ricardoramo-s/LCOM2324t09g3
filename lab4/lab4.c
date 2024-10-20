#include <lcom/lab4.h>
#include <lcom/lcf.h>


#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "../proj/src/drivers/i8042.h"
#include "../proj/src/drivers/mouse.h"
#include "../proj/src/drivers/timer.h"

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need/ it]
    lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab4/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(mouse_test_packet)(uint32_t cnt) {

    if (mouse_write_command(MOUSE_COMM_ENABLE_DATA_REPORTING)) return 1;

    uint8_t mouse_bit_no;

    if (mouse_subscribe(&mouse_bit_no)) return 1;

    int r, ipc_status;
    message msg;
    struct packet packet;

    while (cnt) {

        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                mouse_ih();
                    if (msg.m_notify.interrupts & BIT(2)) {
                        mouse_ih();

                        if (mouse_packet_ready()) {
                            mouse_get_packet(&packet);
                            mouse_print_packet(&packet);
                            cnt--;
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

    if (mouse_unsubscribe()) return 1;

    if (mouse_write_command(MOUSE_COMM_DISABLE_DATA_REPORTING)) return 1;

    return 0;
}

void (mouse_ih)(void) {
    mouse_read_packet();
    return;
}

int(mouse_test_async)(uint8_t idle_time) {
    return 1;
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
