#include <stm32f302xc.h>
#include <stdbool.h>
#include "stm32f3hal/rcc.h"
#include "drivers/leds.h"
#include "drivers/uart.h"

int main(void){
    asm("cpsid i");     /* begin interrupt-sensitive init */ 

    // set up the system and peripheral clocks
    rcc_clock_hse_pll_setup(&rcc_default_config);
    SystemCoreClockUpdate();

    // driver setup
    led_setup();
    uart1_dma_setup();

    asm("cpsie i");     /* end interrupt-sensitive init */
    
    //char array[16];
    //int count = 0;

    while (1) {
        led_toggle(GREEN_LED_PIN);
        uart1_dma_transmit();
        for(volatile int i = 0; i < 5000000; i++);
    }

    // no touchy
    for(volatile int i = 0;;i++) {
        i;
    }
}

void HardFault_Handler(void) {
    led_off(RED_LED_PIN);

    for(;;) {
        // ...
        for (int repeat = 0; repeat < 3; repeat++) {
            led_on(RED_LED_PIN);
            for(volatile int i = 0; i < 500000; i++);
            led_off(RED_LED_PIN);
            for(volatile int i = 0; i < 500000; i++);
        }
        // ---
        for (int repeat = 0; repeat < 3; repeat++) {
            led_on(RED_LED_PIN);
            for(volatile int i = 0; i < 1000000; i++);
            led_off(RED_LED_PIN);
            for(volatile int i = 0; i < 1000000; i++);
        }
        // ...
        for (int repeat = 0; repeat < 3; repeat++) {
            led_on(RED_LED_PIN);
            for(volatile int i = 0; i < 500000; i++);
            led_off(RED_LED_PIN);
            for(volatile int i = 0; i < 500000; i++);
        }
        for(volatile int i = 0; i < 1000000; i++);
    }
}
