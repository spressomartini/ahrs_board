#pragma once
#include "stm32f3hal/gpio.h"

#define GREEN_LED_PIN   (0u)
#define RED_LED_PIN     (1u)

void led_setup(void);
void led_on(uint32_t led);
void led_off(uint32_t led);
void led_toggle(uint32_t led);
