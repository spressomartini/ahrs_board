#include "devices/bmx055.h"
#include "stm32f3hal/gpio.h"
#include "stm32f3hal/i2c.h"

static GPIO_PIN_Config bmx055_pow_config = {
    .base = GPIOA,
    .pin = BMX055_POW_PIN,
    .mode = GPIO_MODER_OUTPUT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_LOW,
};

static GPIO_PIN_Config i2c1_scl_pin_config = {
    .base = GPIOB,
    .pin = 6,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 4,
};

static GPIO_PIN_Config i2c1_sda_pin_config = {
    .base = GPIOB,
    .pin = 7,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 4,
};

void bmx055_setup(void) {
    gpio_pin_setup(&bmx055_pow_config);
    gpio_pin_setup(&i2c1_scl_pin_config);
    gpio_pin_setup(&i2c1_sda_pin_config);
    i2c_master_setup(&i2c_100khz_config);
}

void bmx055_on(void) {
    gpio_write_pin(GPIOA, BMX055_POW_PIN, GPIO_PIN_RESET);
    for (volatile int i = 0; i < 1000000; i++);
}

void bmx055_off(void) {
    gpio_write_pin(GPIOA, BMX055_POW_PIN, GPIO_PIN_SET);
}

void bmx055_read_accel_whoami(uint8_t *buf) {
    uint8_t reg[] = {BMX055_ACCEL_BGW_CHIPID_ADDR};
    i2c_master_transfer7(I2C1, BMX055_ACCEL_I2C_ADDR, reg, 1, buf, 1);
}

void bmx055_read_gyro_whoami(uint8_t *buf) {
    uint8_t reg[] = {BMX055_GYRO_BGW_CHIPID_ADDR};
    i2c_master_transfer7(I2C1, BMX055_GYRO_I2C_ADDR, reg, 1, buf, 1);
}

void bmx055_read_magnet_whoami(uint8_t *buf) {
    uint8_t reg[] = {BMX055_MAGNET_BGW_CHIPID_ADDR};
    i2c_master_transfer7(I2C1, BMX055_MAGNET_I2C_ADDR, reg, 1, buf, 1);
}
