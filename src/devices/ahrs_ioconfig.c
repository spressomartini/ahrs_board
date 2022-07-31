#include "devices/ahrs_ioconfig.h"

/* ==================
 *  UART GPIO CONFIG
 * ==================*/
GPIO_PIN_Config usart1tx_pin_config = {
    .base = GPIOA,
    .pin = 9,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 7,
};

GPIO_PIN_Config usart1rx_pin_config = {
    .base = GPIOA,
    .pin = 10,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 7,
};

/* =================
 *  LED GPIO CONFIG
 * =================*/
GPIO_PIN_Config green_led_config = {
    .base = GPIOB,
    .pin = GREEN_LED_PIN,
    .mode = GPIO_MODER_OUTPUT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_LOW,
};

GPIO_PIN_Config red_led_config = {
    .base = GPIOB,
    .pin = RED_LED_PIN,
    .mode = GPIO_MODER_OUTPUT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_LOW,
};

/* ====================
 *  BMX055 GPIO CONFIG
 * ====================*/
GPIO_PIN_Config bmx055_pow_config = {
    .base = GPIOA,
    .pin = BMX055_POW_PIN,
    .mode = GPIO_MODER_OUTPUT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_LOW,
};

GPIO_PIN_Config i2c1_scl_pin_config = {
    .base = GPIOB,
    .pin = 6,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 4,
};

GPIO_PIN_Config i2c1_sda_pin_config = {
    .base = GPIOB,
    .pin = 7,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 4,
};
