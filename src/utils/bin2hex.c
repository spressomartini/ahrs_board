#include "utils/bin2hex.h"

static char lut[] = "0123456789ABCDEF";

uint16_t bin2hex(uint8_t byte) {
    uint16_t result = (lut[byte >> 4] << 8) | (lut[byte & 0xFu]);
    return result;
}
