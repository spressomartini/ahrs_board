#include "stm32f3hal/gpio.h"
#include "drivers/leds.h"

// GREEN LED CONFIG
static GPIO_PIN_Config green_led_config = {
    .base = GPIOB,
    .pin = GREEN_LED_PIN,
    .mode = GPIO_MODER_OUTPUT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_LOW,
};

// RED LED CONFIG
static GPIO_PIN_Config red_led_config = {
    .base = GPIOB,
    .pin = RED_LED_PIN,
    .mode = GPIO_MODER_OUTPUT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_LOW,
};

void led_setup(void) {
    gpio_pin_setup(&green_led_config);
    gpio_pin_setup(&red_led_config);
}

/* AHRS BOARD SPECIFIC FOR GPIOB LEDs */
void led_on(uint32_t led) {
    gpio_write_pin(GPIOB, led, GPIO_PIN_SET);
}

void led_off(uint32_t led) {
    gpio_write_pin(GPIOB, led, GPIO_PIN_RESET);
}

void led_toggle(uint32_t led) {
    if (gpio_read_pin_output(GPIOB, led)) {
        gpio_write_pin(GPIOB, led, GPIO_PIN_RESET);
    }
    else {
        gpio_write_pin(GPIOB, led, GPIO_PIN_SET);
    }
}
