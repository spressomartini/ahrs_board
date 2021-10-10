#include <stm32f302xc.h>
#include <stdbool.h>
#include "stm32f3hal/rcc.h"
#include "stm32f3hal/usart.h"
#include "stm32f3hal/gpio.h"
#include "drivers/leds.h"
#include "drivers/uart.h"

int main(void){
    // begin interrupt-sensitive init
    asm("cpsid i");
    // set up the system and peripheral clocks
    rcc_clock_hse_pll_setup(&rcc_default_config);
    SystemCoreClockUpdate();

    // set up the LEDs
    led_setup();

    // set up UART1
    uart1_setup();
    asm("cpsie i");

    // WRITE TO LEDS
    gpio_write_pin(GPIOB, GREEN_LED_PIN, GPIO_PIN_SET);
    gpio_write_pin(GPIOB, RED_LED_PIN, GPIO_PIN_SET);
    for(volatile int i = 0; i < 100000; i++);
    gpio_write_pin(GPIOB, 0, GPIO_PIN_RESET);
    gpio_write_pin(GPIOB, 1, GPIO_PIN_RESET);

    // UART LOOPBACK
    uint8_t c = 0;
    for(;;) {
        while(!usart_rdr_not_empty(USART1));
        c = USART1->RDR & 0xFF;
        while(!usart_tdr_empty(USART1));
        USART1->TDR = c;
    }

    // no touchy
    for(volatile int i = 0;;i++) {
        i;
    }
}

void HardFault_Handler(void) {
    for(;;) {
        GPIOB->ODR = 2;
        for(volatile int i = 0; i < 1000000; i++);
        GPIOB->ODR = 0;
        for(volatile int i = 0; i < 1000000; i++);
    }
}
