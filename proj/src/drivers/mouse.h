#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "kbc.h"
#include "i8042.h"


int mouse_subscribe(uint8_t *bit_no);
int mouse_unsubscribe();

int mouse_enable_data();
int mouse_disable_data();

int mouse_read_packet();
int mouse_get_packet(struct packet *out);
bool mouse_packet_ready();

int mouse_stream_mode_command(uint8_t command);
int mouse_write_command(uint8_t command);
int mouse_read_response(uint8_t *response);
