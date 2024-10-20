#pragma once

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "utils.h"

int timer_subscribe(uint8_t *bit_no);

int timer_unsubscribe();
