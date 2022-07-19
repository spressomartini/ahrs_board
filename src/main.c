#include <stm32f302xc.h>
#include <stdbool.h>
#include "stm32f3hal/rcc.h"
#include "utils/bin2hex.h"
#include "drivers/leds.h"
#include "drivers/uart.h"
#include "devices/bmx055.h"
#include "cm4/systick.h"

#define TSTRING1_LEN    (28 * 16)
#define TSTRING2_LEN    (14)

const char teststring1[] = ""
"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\r";

const char teststring2[] = "Hello World!\n\r";
const char newline[] = "\n\r";

int main(void){
    uint32_t primask = __get_PRIMASK();
    __disable_irq();        /* begin interrupt-sensitive init */

    // set up the system and peripheral clocks
    rcc_clock_hse_pll_setup(&rcc_default_config);
    SystemCoreClockUpdate();

    // driver setup
    led_setup();
    uart1_dma_setup();
    systick_setup(30, 4u, 0u);

    // device setup
    bmx055_setup();
    bmx055_on();

    __set_PRIMASK(primask); /* end interrupt-sensitive init */

    uart1_queue_transmit(teststring1, TSTRING1_LEN);

    uint8_t whoami;
    uint16_t hex;
    char whoami_str[4] = "0x??";
    while (1) {
        led_toggle(GREEN_LED_PIN);
        bmx055_read_accel_whoami(&whoami);
        hex = bin2hex(whoami);
        whoami_str[2] = (hex >> 8) & 0xFFu;
        whoami_str[3] = (hex & 0xFFu);
        uart1_queue_transmit(whoami_str, 4);
        uart1_queue_transmit(newline, 2);
        
        bmx055_read_gyro_whoami(&whoami);
        hex = bin2hex(whoami);
        whoami_str[2] = (hex >> 8) & 0xFFu;
        whoami_str[3] = (hex & 0xFFu);
        uart1_queue_transmit(whoami_str, 4);
        uart1_queue_transmit(newline, 2);

        bmx055_read_magnet_whoami(&whoami);
        hex = bin2hex(whoami);
        whoami_str[2] = (hex >> 8) & 0xFFu;
        whoami_str[3] = (hex & 0xFFu);
        uart1_queue_transmit(whoami_str, 4);
        uart1_queue_transmit(newline, 2);

        for(volatile int i = 0; i < 1000000; i++);
    }

    // no touchy
    for(volatile int i = 0;;i++) {
        i;
    }
}

void SysTick_Handler(void) {
    // put periodic functions here
    led_toggle(RED_LED_PIN);
    uart1_dma_fsm();
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
