#include "devices/bmx055.c"
#include "stm32f3hal/gpio.h"

static GPIO_PIN_Config bmx055_pow_config = {
    .base = GPIOA,
    .pin = BMX055_POW_PIN,
    .mode = GPIO_MODER_OUTPUT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_LOW,
};

void bmx055_pow_setup(void) {
    gpio_pin_setup(&bmx055_pow_config);
}

void bmx055_on() {
    gpio_Write_pin(GPIOA, BMX055_POW_PIN, GPIO_PIN_RESET);
}

void bmx055_off() {
    gpio_Write_pin(GPIOA, BMX055_POW_PIN, GPIO_PIN_SET);
}

