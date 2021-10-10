#include "stm32f3hal/gpio.h"
#include "drivers/leds.h"

// GREEN LED CONFIG
static GPIO_PIN_Config green_led_config = {
    .base = GPIOB,
    .pin = 0,
    .mode = GPIO_MODER_OUTPUT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_LOW,
};

// RED LED CONFIG
static GPIO_PIN_Config red_led_config = {
    .base = GPIOB,
    .pin = 1,
    .mode = GPIO_MODER_OUTPUT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_LOW,
};

void led_setup(void) {
    gpio_pin_setup(&green_led_config);
    gpio_pin_setup(&red_led_config);
}
