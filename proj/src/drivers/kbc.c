#include "kbc.h"

int kbc_read_status(uint8_t *status) {

  if (util_sys_inb(KBC_REG_STAT, status)) return 1;

  return 0;
}

int kbc_read_outbuff(uint8_t *data, bool mouse) {

  uint8_t status;
  uint8_t attempts = 8;

  while (attempts) {
    attempts--;

    if (kbc_read_status(&status)) {
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }

    if (((status & KBC_STAT_AUX) && !mouse)) {
      printf("[kbc_read_outbuff] keyboard packet mismatch\n");
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }

    if ((!(status & KBC_STAT_AUX) && mouse)) {
      printf("[kbc_read_outbuff] mouse packet mismatch\n");
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }

    if (status & KBC_STAT_OBF) {
      if (util_sys_inb(KBC_REG_OUT_BUF, data)) {
        printf("[kbc_read_outbuff] failed sys_inb\n");
        tickdelay(micros_to_ticks(DELAY_US));
        continue;
      }

      if (status & (KBC_STAT_PARITY | KBC_STAT_TIMEOUT)) {
        printf("[kbc_read_outbuff] pairity or timeout\n");
        return 1;
      }
      else
        return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

int kbc_write(uint8_t port, uint8_t command) {

  uint8_t status;
  uint8_t cnt = MAX_ATTEMPTS;

  while (cnt) {
    if (kbc_read_status(&status)) {
      printf("[kbc_write] cannot read status\n");
      return 1;
    }

    if (status & KBC_STAT_IBF) {
      tickdelay(micros_to_ticks(DELAY_US));
      cnt--;
      continue;
    }

    if (sys_outb(port, command)) {
      printf("[kbc_write] could not sys_outb to port\n");
      return 1;
    }

    return 0;
  }

  printf("[kbc_write] Could not write command (%u) to kbc\n", command);
  return 1;
}
