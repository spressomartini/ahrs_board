#pragma once
#include <stm32f302xc.h>

#define BMX055_POW_PIN  (5u)
#define BMX055_ACCEL_I2C_ADDR       (0x18u)
#define BMX055_GYRO_I2C_ADDR        (0x68u)
#define BMX055_MAGNET_I2C_ADDR      (0x10u)

#define BMX055_ACCEL_BGW_CHIPID_ADDR    (0x00)
#define BMX055_GYRO_BGW_CHIPID_ADDR     (0x00)
#define BMX055_MAGNET_BGW_CHIPID_ADDR   (0x00)


void bmx055_setup(void);
void bmx055_on(void);
void bmx055_off(void);
void bmx055_read_accel_whoami(uint8_t *buf);
void bmx055_read_gyro_whoami(uint8_t *buf);
void bmx055_read_magnet_whoami(uint8_t *buf);
