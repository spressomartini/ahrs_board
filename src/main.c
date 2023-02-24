#include <stm32f302xc.h>
#include <stdbool.h>
#include "stm32f3hal/rcc.h"
#include "utils/bin2hex.h"
#include "utils/aproto.h"
#include "utils/mpack.h"
#include "drivers/leds.h"
#include "drivers/uart.h"
#include "devices/bmx055.h"
#include "devices/ahrs_ioconfig.h"
#include "algos/quaternion.h"
#include "algos/madgwick.h"
#include "cm4/systick.h"
#define VERBOSE

#define INIT_MSG_LEN    (49)
#define NEWLINE_LEN     (2)
const char INIT_MSG_BUF[] = "Successfully initialized peripherals and devices!";
const char NEWLINE_BUF[] = "\n\r";

BMX055_AccelData_t accel_data;
BMX055_GyroData_t gyro_data;
BMX055_MagnetData_t magnet_data;

// systick flags
volatile bool led_toggle_flag = false;
volatile bool flush_uart_flag = false;
volatile bool read_data_flag = false;

static inline void main_init(void) {
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
}

static inline void bmx_diagnostics(void) {
    uint8_t whoami;
    uint16_t hex;
    char whoami_str[4] = "0x??";

    // toggle on the green led
    led_toggle(GREEN_LED_PIN);

    // try contacting accelerometer
    bmx055_read_accel_whoami(&whoami);
    hex = bin2hex(whoami);
    whoami_str[2] = (hex >> 8) & 0xFFu;
    whoami_str[3] = (hex & 0xFFu);
    uart1_queue_transmit(whoami_str, 4);
    uart1_queue_transmit(NEWLINE_BUF, NEWLINE_LEN);

    // try contacting gyroscope
    bmx055_read_gyro_whoami(&whoami);
    hex = bin2hex(whoami);
    whoami_str[2] = (hex >> 8) & 0xFFu;
    whoami_str[3] = (hex & 0xFFu);
    uart1_queue_transmit(whoami_str, 4);
    uart1_queue_transmit(NEWLINE_BUF, NEWLINE_LEN);

    // try contacting magnetometer
    bmx055_read_magnet_whoami(&whoami);
    hex = bin2hex(whoami);
    whoami_str[2] = (hex >> 8) & 0xFFu;
    whoami_str[3] = (hex & 0xFFu);
    uart1_queue_transmit(whoami_str, 4);
    uart1_queue_transmit(NEWLINE_BUF, NEWLINE_LEN);

    // toggle off green led
    led_toggle(GREEN_LED_PIN);
}

int main(void) {
    euler_angle eu_debug;

    // mpack
    char mpack_buf[64];
    size_t used;
    mpack_writer_t writer;

    // bring up the peripherals and devices
    main_init();

#ifdef VERBOSE
    // transmit a startup message over uart
    uart1_queue_transmit(INIT_MSG_BUF, INIT_MSG_LEN);
    uart1_queue_transmit(NEWLINE_BUF, NEWLINE_LEN);

    // run bmx055 diagnostics
    bmx_diagnostics();
#endif

    // XXX figure out why you put this here and not with the main init
    bmx055_magnet_setup();

    while (1) {
        if (led_toggle_flag) {
            led_toggle(RED_LED_PIN);
            led_toggle_flag = false;
        }

        if (flush_uart_flag) {
            uart1_dma_fsm();
            flush_uart_flag = false;
        }

        if (read_data_flag) {
            bmx055_accel_burst_read_data(&accel_data);
            bmx055_gyro_burst_read_data(&gyro_data);
            bmx055_magnet_burst_read_data(&magnet_data);
            read_data_flag = false;

            // filter
            imu_filter(accel_data.x, accel_data.y, accel_data.z, gyro_data.x, gyro_data.y, gyro_data.z);
            eu_debug = q_toeuler(&q_est);

            // messagepack
            mpack_writer_init(&writer, mpack_buf, 64);
            mpack_start_map(&writer, 1);
            mpack_write_cstr(&writer, "ax");
            mpack_write_u16(&writer, accel_data.x);
            mpack_write_cstr(&writer, "ay");
            mpack_write_u16(&writer, accel_data.y);
            mpack_write_cstr(&writer, "az");
            mpack_write_u16(&writer, accel_data.z);
            mpack_finish_map(&writer);
            used = mpack_writer_buffer_used(&writer);
            mpack_writer_destroy(&writer);
            uart1_queue_transmit(mpack_buf, used);
        }
    }

    // no touchy
    for(volatile int i = 0;;i++) {
        i;
    }
}

void SysTick_Handler(void) {
    // flags to signal functionality in runloop
    led_toggle_flag = true;
    flush_uart_flag = true;
    read_data_flag = true;
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
