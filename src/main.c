#include <stm32f302xc.h>
#include <stdbool.h>
#include "stm32f3hal/rcc.h"
#include "stm32f3hal/usart.h"
#include "drivers/leds.h"
#include "drivers/uart.h"

int main(void){
    asm("cpsid i");     /* begin interrupt-sensitive init */ 

    // set up the system and peripheral clocks
    rcc_clock_hse_pll_setup(&rcc_default_config);
    SystemCoreClockUpdate();

    // driver setup
    led_setup();
    uart1_setup();

    asm("cpsie i");     /* end interrupt-sensitive init */
    
    // WRITE TO LEDS
    while (1) {
        led_toggle(RED_LED_PIN);
        for(volatile int i = 0; i < 1000000; i++);
    }
    led_on(RED_LED_PIN);
    led_off(RED_LED_PIN);

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
