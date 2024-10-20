#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "kbc.h"

int keyboard_subscribe(uint8_t *bit_no);

int keyboard_unsubscribe();

int keyboard_enable_interrupts();

int keyboard_read_scancode(uint8_t* scancode);

int keyboard_write_command(uint8_t command);
