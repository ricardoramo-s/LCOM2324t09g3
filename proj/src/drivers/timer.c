#include "timer.h"

size_t timer_counter = 0;
static int timer_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
    if (freq < 19 || freq > TIMER_FREQ) return 1;

    uint8_t st, control, msb, lsb, timerADDR;
    timerADDR = TIMER_0 + timer;

    if (timer_get_conf(timer, &st)) return 1;

    control = TIMER_LSB_MSB | (timer << 6) | (st & 0x0f);
    uint16_t f = (uint16_t) (TIMER_FREQ / freq);
    (util_get_16_LSB)(f, &lsb);
    (util_get_16_MSB)(f, &msb);

    if (sys_outb(TIMER_CTRL, control)) return 1;

    if (sys_outb(timerADDR, lsb)) return 1;

    if (sys_outb(timerADDR, msb)) return 1;

    return 0;
}

int timer_subscribe(uint8_t *bit_no) {
  
    *bit_no = BIT(timer_hook_id);

    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id)) return 1;

    return 0;
}

int timer_unsubscribe() {
  
    if (sys_irqrmpolicy(&timer_hook_id)) return 1;

    return 0;
}

void (timer_int_handler)() {
    timer_counter++;
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
    uint8_t control = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

    if (sys_outb(TIMER_CTRL, control)) return 1;

    if (util_sys_inb(TIMER_0 + timer, st)) return 1;

    return 0;
}


int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  
    union timer_status_field_val value;
  
    switch (field) {

        case tsf_all:
        value.byte = st;
        break;

        case tsf_initial:
        value.in_mode = (st >> 4) & 0x3;
        break;

        case tsf_mode:
        value.count_mode = (st >> 1) & 0x07;
        break;

        case tsf_base:
        value.bcd = st & 0x01;

    }

    if (timer_print_config(timer, field, value)) return 1;

    return 0;
}
