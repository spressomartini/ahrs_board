#include "stm32f3hal/usart.h"
#include "stm32f3hal/gpio.h"
#include "stm32f3hal/rcc.h"
#include "drivers/uart.h"

// USART1 PIN CONFIG
static GPIO_PIN_Config usart1tx_pin_config = {
    .base = GPIOA,
    .pin = 9,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 7,
};

static GPIO_PIN_Config usart1rx_pin_config = {
    .base = GPIOA,
    .pin = 10,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 7,
};

static USART_Config usart1_config = {
    .base = USART1,
    .baud = 115200,
    .clkf = 64000000,
    .stopbits = 0,
    .wordlen = USART_8DATABITS,
};

void uart1_setup(void) {
    gpio_pin_setup(&usart1tx_pin_config);
    gpio_pin_setup(&usart1rx_pin_config);
    usart_setup(&usart1_config);
}
