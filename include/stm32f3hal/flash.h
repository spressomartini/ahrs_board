#pragma once
#include <stm32f302xc.h>

void flash_enable_prefetch(void);
void flash_set_latency(uint32_t ws);
