#include "stm32f3hal/gpio.h"
#include "drivers/leds.h"
#include "devices/ahrs_ioconfig.h"

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
