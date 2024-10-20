#include "mouse.h"
#include <stdbool.h>

static int mouse_hook_id = 2;
bool mouse_ih_valid;
uint8_t data_packet;

uint8_t current_byte_index = 0;
uint8_t current_byte;
uint8_t bytes[3];
bool packet_ready = false;
struct packet packet;

int mouse_subscribe(uint8_t *bit_no) {

  *bit_no = BIT(mouse_hook_id);

  if (sys_irqsetpolicy(KBC_IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)) return 1;

  return 0;
}

int mouse_unsubscribe() {

  if (sys_irqrmpolicy(&mouse_hook_id)) return 1;

  return 0;
}

bool mouse_packet_ready() {
  return packet_ready;
}

int mouse_read_packet() {
  if (kbc_read_outbuff(&current_byte, true)) return 1;

  if (current_byte_index == 0 && current_byte & BIT(3)) {
    bytes[0] = current_byte;
    current_byte_index++;
    packet_ready = false;
  }
  else if (current_byte_index == 1) {
    bytes[1] = current_byte;
    current_byte_index++;
  }
  else if (current_byte_index == 2) {
    bytes[2] = current_byte;
    current_byte_index = 0;
    packet_ready = true;
  }

  return 0;
}

int mouse_get_packet(struct packet *out) {
  if (!packet_ready) return 1; // not valid
  memcpy(packet.bytes, bytes, 3);

  packet.lb = bytes[0] & MOUSE_DP_LB;
  packet.rb = bytes[0] & MOUSE_DP_RB;
  packet.mb = bytes[0] & MOUSE_DP_MB;

  packet.x_ov = bytes[0] & MOUSE_DP_X_OVFL;
  packet.y_ov = bytes[0] & MOUSE_DP_Y_OVFL;
  packet.delta_x = (bytes[0] & MOUSE_DP_MSB_X) ? (0xFF00 | bytes[1]) : bytes[1];
  packet.delta_y = (bytes[0] & MOUSE_DP_MSB_Y) ? (0xFF00 | bytes[2]) : bytes[2];

  memcpy(out, &packet, sizeof(struct packet));
  return 0;
}

int mouse_read_response(uint8_t *response) {
  uint8_t status;
  uint8_t cnt = MAX_ATTEMPTS;

  while (cnt) {

    cnt--;

    if (kbc_read_status(&status)) return 1;

    if (!(status & KBC_STAT_OBF)) {
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }

    if (kbc_read_outbuff(response, true)) return 1;

    return 0;
  }

  printf("[mouse_read_response] Could not read mouse response\n");

  return 1;
}

int mouse_enable_data() {
  if (mouse_write_command(MOUSE_COMM_ENABLE_DATA_REPORTING)) return 1;

  return 0;
}

int mouse_disable_data() {
  if (mouse_write_command(MOUSE_COMM_DISABLE_DATA_REPORTING)) return 1;

  return 0;
}

int mouse_stream_mode_command(uint8_t command) {
  if (mouse_disable_data()) return 1;

  if (mouse_write_command(command)) {
    return 1;
  }

  if (mouse_enable_data()) return 1;

  return 0;
}

int mouse_write_command(uint8_t command) {
  uint8_t aknl;
  uint8_t cnt = MAX_ATTEMPTS;

  while (cnt && aknl != MOUSE_RES_ACK) {
    if (kbc_write(KBC_REG_COMMAND, KBC_COMM_WRITE_MOUSE_COMM_BYTE)) return 1;
    if (kbc_write(KBC_REG_IN_BUF, command)) return 1;

    tickdelay(micros_to_ticks(DELAY_US));

    if (util_sys_inb(KBC_REG_OUT_BUF, &aknl)) return 1;
    if (aknl == MOUSE_RES_ACK) return 0;
    cnt--;
  }

  printf("[mouse_write_command] Could not write command (%u) to mouse\n", command);
  return 1;
}
