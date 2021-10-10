#include "stm32f3hal/flash.h"
#include <stm32f302xc.h>

void flash_enable_prefetch(void) {
    // enable flash prefetch
    FLASH->ACR |= FLASH_ACR_PRFTBE;
}

void flash_set_latency(uint32_t ws) {
    // read FLASH ACR
    uint32_t reg = FLASH->ACR;

    // clear [2:0] and set latency
    reg &= ~FLASH_ACR_LATENCY;
    reg |= ws;

    // write into ACR
    FLASH->ACR = reg;
}
