#pragma once
#include "stm32f3hal/gpio.h"

void led_setup(void);
void led_on(uint32_t led);
void led_off(uint32_t led);
void led_toggle(uint32_t led);
