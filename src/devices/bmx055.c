#include "stm32f3hal/i2c.h"
#include "devices/bmx055.h"
#include "devices/ahrs_ioconfig.h"

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

/* =======================
 * ACCELEROMETER FUNCTIONS
 * =======================*/
void bmx055_read_accel_whoami(uint8_t *buf) {
    uint8_t reg[] = {BMX055_ACCEL_CHIPID_ADDR};
    i2c_master_transfer7(I2C1, BMX055_ACCEL_I2C_ADDR, reg, 1, buf, 1);
}

void bmx055_accel_burst_read_data(BMX055_AccelData_t *buf) {
    uint8_t reg[] = {BMX055_ACCEL_ACCD_X_LSB_ADDR};
    i2c_master_transfer7(I2C1, BMX055_ACCEL_I2C_ADDR, reg, 1, buf->array, BMX055_ACCEL_DATA_SIZE);
}

/* ===================
 * GYROSCOPE FUNCTIONS
 * ===================*/
void bmx055_read_gyro_whoami(uint8_t *buf) {
    uint8_t reg[] = {BMX055_GYRO_CHIPID_ADDR};
    i2c_master_transfer7(I2C1, BMX055_GYRO_I2C_ADDR, reg, 1, buf, 1);
}

void bmx055_gyro_burst_read_data(BMX055_GyroData_t *buf) {
    uint8_t reg[] = {BMX055_GYRO_RATE_X_LSB_ADDR};
    i2c_master_transfer7(I2C1, BMX055_GYRO_I2C_ADDR, reg, 1, buf->array, BMX055_GYRO_DATA_SIZE);
}

/* ======================
 * MAGNETOMETER FUNCTIONS
 * ======================*/
void bmx055_read_magnet_whoami(uint8_t *buf) {
    uint8_t on[] = {BMX055_MAGNET_4B_ADDR, 0x01u};
    uint8_t reg[] = {BMX055_MAGNET_CHIPID_ADDR};
    i2c_master_transfer7(I2C1, BMX055_MAGNET_I2C_ADDR, on, 2, 0, 0);
    for (volatile int i = 0; i < 5000; i++);
    i2c_master_transfer7(I2C1, BMX055_MAGNET_I2C_ADDR, reg, 1, buf, 1);
}

void bmx055_magnet_setup() {
    uint8_t config[] = {BMX055_MAGNET_4C_ADDR, 0b00111000u};
    i2c_master_transfer7(I2C1, BMX055_MAGNET_I2C_ADDR, config, 2, 0, 0);
    for (volatile int i = 0; i < 5000; i++);
}

void bmx055_magnet_burst_read_data(BMX055_MagnetData_t *buf) {
    uint8_t reg[] = {BMX055_MAGNET_DATA_X_LSB_ADDR};
    i2c_master_transfer7(I2C1, BMX055_MAGNET_I2C_ADDR, reg, 1, buf->array, BMX055_MAGNET_DATA_SIZE);
}
