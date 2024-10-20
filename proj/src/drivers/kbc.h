#pragma once

#include <lcom/lcf.h>
#include <stdint.h>

#include "utils.h"
#include "i8042.h"


int kbc_read_status(uint8_t *status);

int kbc_read_outbuff(uint8_t *data, bool mouse);

int kbc_write(uint8_t port, uint8_t command);
