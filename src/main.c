#include <stm32f302xc.h>
#include <stdbool.h>
#include "stm32f3hal/rcc.h"
#include "drivers/leds.h"
#include "drivers/uart.h"

#define TSTRING1_LEN    (28 * 16)
#define TSTRING2_LEN    (14)

const char teststring1[] = "" 
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r"
"abcdefghijklmnopqrstuvwxyz\n\r";

const char teststring2[] = "Hello World!\n\r";

int main(void){
    uint32_t primask = __get_PRIMASK();
    __disable_irq();        /* begin interrupt-sensitive init */

    // set up the system and peripheral clocks
    rcc_clock_hse_pll_setup(&rcc_default_config);
    SystemCoreClockUpdate();

    // driver setup
    led_setup();
    uart1_dma_setup();

    __set_PRIMASK(primask); /* end interrupt-sensitive init */

    uart1_queue_transmit(teststring1, TSTRING1_LEN);
    
    while (1) {
        led_toggle(GREEN_LED_PIN);
        uart1_dma_fsm();
        uart1_queue_transmit(teststring2, TSTRING2_LEN);
        for(volatile int i = 0; i < 500000; i++);
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
