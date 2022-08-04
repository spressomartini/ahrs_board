#include <stm32f302xc.h>
#include <stdbool.h>
#include "stm32f3hal/rcc.h"
#include "utils/bin2hex.h"
#include "utils/aproto.h"
#include "drivers/leds.h"
#include "drivers/uart.h"
#include "devices/bmx055.h"
#include "devices/ahrs_ioconfig.h"
#include "algos/quaternion.h"
#include "algos/madgwick.h"
#include "cm4/systick.h"

#define TSTRING1_LEN    (28 * 16)

const char teststring1[] = ""
"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\r";

const char newline[] = "\n\r";

BMX055_AccelData_t accel_data;
BMX055_GyroData_t gyro_data;
BMX055_MagnetData_t magnet_data;
bool fresh_data = false;

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
    euler_angle eu_debug;
    aproto_t aproto_packet;

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

    bmx055_magnet_setup();

    while (1) {
        if (fresh_data) {
            imu_filter(accel_data.x, accel_data.y, accel_data.z, gyro_data.x, gyro_data.y, gyro_data.z);
            eu_debug = q_toeuler(&q_est);
            pack_aproto(&aproto_packet, accel_data.x, accel_data.y, accel_data.z, gyro_data.x, gyro_data.y, gyro_data.z,
                        magnet_data.x, magnet_data.y, magnet_data.z, q_est.a, q_est.b, q_est.c, q_est.d,
                        eu_debug.roll, eu_debug.pitch, eu_debug.yaw);
            uart1_queue_transmit((char *)&aproto_packet, APROTO_PACKET_SIZE);
            fresh_data = false;
        }
        //for(volatile int i = 0; i < 1000000; i++);
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
    bmx055_accel_burst_read_data(&accel_data);
    bmx055_gyro_burst_read_data(&gyro_data);
    bmx055_magnet_burst_read_data(&magnet_data);
    fresh_data = true;
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
