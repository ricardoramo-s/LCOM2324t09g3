#pragma once

#include <lcom/lcf.h>
#include <stdint.h>

int util_sys_inb(int port, uint8_t *value);

int util_get_16_LSB(uint16_t val, uint8_t *lsb);

int util_get_16_MSB(uint16_t val, uint8_t *msb);
