#include "keyboard.h"

static int kbd_hook_id = 1;
bool kbd_ih_valid;
uint8_t scan_code;

int keyboard_subscribe(uint8_t *bit_no) {

  *bit_no = BIT(kbd_hook_id);

  if (sys_irqsetpolicy(KBC_IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id)) return 1;

  return 0;
}

int keyboard_unsubscribe() {

  if (sys_irqrmpolicy(&kbd_hook_id)) return 1;

  return 0;
}

int keyboard_read_scancode(uint8_t *scancode) {
  uint8_t status;

  if (kbc_read_status(&status)) return 1;
  if (!(status & KBC_STAT_OBF)) return 1;
  if (status & (KBC_STAT_PARITY | KBC_STAT_TIMEOUT)) return 1;

  if (kbc_read_outbuff(scancode, false)) return 1;

  return 0;
}

void(kbc_ih)() {

  uint8_t status, invalid;

  invalid = KBC_STAT_PARITY | KBC_STAT_TIMEOUT;

  if (kbc_read_status(&status)) return;

  if (!(status & KBC_STAT_OBF)) return;

  if (status & invalid) return;

  if (kbc_read_outbuff(&scan_code, false)) return;

  kbd_ih_valid = 1;
}

int keyboard_enable_interrupts() {

  uint8_t command;
	if (kbc_write(KBC_REG_COMMAND, KBC_COMM_DISABLE_KBD_INTERFACE)) return 1;
  if (kbc_write(KBC_REG_COMMAND, KBC_COMM_READ_BYTE)) return 1;
  if (kbc_read_outbuff(&command, false)) return 1;

  command |= KBC_COMM_B_INT_KBD;

  if (kbc_write(KBC_REG_COMMAND, KBC_COMM_WRITE_BYTE)) return 1;
  if (kbc_write(KBC_REG_IN_BUF, command)) return 1;
	if (kbc_write(KBC_REG_COMMAND, KBC_COMM_ENABLE_KBD_INTERFACE)) return 1;

  return 0;
}
