#include "utils.h"

int util_sys_inb(int port, uint8_t *value) {

    uint32_t val;

    if (sys_inb(port, &val)) return 1;

    *value = val;

    return 0;
}

int util_get_16_LSB(uint16_t val, uint8_t *lsb) {

    *lsb = (uint8_t) val;

    return 0;
}

int util_get_16_MSB(uint16_t val, uint8_t *msb) {

    *msb = (uint8_t) (val >> 8);

    return 1;
}
