#pragma once
#include <stdint.h>

#define BMX055_ACCEL_I2C_ADDR       (0x18u)
#define BMX055_GYRO_I2C_ADDR        (0x68u)
#define BMX055_MAGNET_I2C_ADDR      (0x10u)

#define BMX055_ACCEL_DATA_SIZE      (7u)
#define BMX055_GYRO_DATA_SIZE       (6u)
#define BMX055_MAGNET_DATA_SIZE     (8u)

// ACCEL REGISTER ADDRESSES
typedef enum {
    BMX055_ACCEL_CHIPID_ADDR = 0x00,
    BMX055_ACCEL_ACCD_X_LSB_ADDR = 0x02,
    BMX055_ACCEL_ACCD_X_MSB_ADDR,
    BMX055_ACCEL_ACCD_Y_LSB_ADDR,
    BMX055_ACCEL_ACCD_Y_MSB_ADDR,
    BMX055_ACCEL_ACCD_Z_LSB_ADDR,
    BMX055_ACCEL_ACCD_Z_MSB_ADDR,
    BMX055_ACCEL_ACCD_TEMP_ADDR,
    BMX055_ACCEL_INT_STATUS_0_ADDR,
    BMX055_ACCEL_INT_STATUS_1_ADDR,
    BMX055_ACCEL_INT_STATUS_2_ADDR,
    BMX055_ACCEL_INT_STATUS_3_ADDR,
    BMX055_ACCEL_FIFO_STATUS_ADDR = 0x0E,
    BMX055_ACCEL_PMU_RANGE_ADDR,
    BMX055_ACCEL_PMU_BW_ADDR,
    BMX055_ACCEL_PMU_LPW_ADDR,
    BMX055_ACCEL_PMU_LOW_POWER_ADDR,
    BMX055_ACCEL_ACCD_HBW_ADDR,
    BMX055_ACCEL_BGW_SOFTRESET_ADDR,
    BMX055_ACCEL_INT_EN_0_ADDR = 0x16,
    BMX055_ACCEL_INT_EN_1_ADDR,
    BMX055_ACCEL_INT_EN_2_ADDR,
    BMX055_ACCEL_INT_MAP_0_ADDR,
    BMX055_ACCEL_INT_MAP_1_ADDR,
    BMX055_ACCEL_INT_MAP_2_ADDR,
    BMX055_ACCEL_INT_SRC_ADDR = 0x1E,
    BMX055_ACCEL_INT_OUT_CTRL_ADDR = 0x20,
    BMX055_ACCEL_INT_RST_LATCH_ADDR,
    BMX055_ACCEL_INT_0_ADDR,
    BMX055_ACCEL_INT_1_ADDR,
    BMX055_ACCEL_INT_2_ADDR,
    BMX055_ACCEL_INT_3_ADDR,
    BMX055_ACCEL_INT_4_ADDR,
    BMX055_ACCEL_INT_5_ADDR,
    BMX055_ACCEL_INT_6_ADDR,
    BMX055_ACCEL_INT_7_ADDR,
    BMX055_ACCEL_INT_8_ADDR,
    BMX055_ACCEL_INT_9_ADDR,
    BMX055_ACCEL_INT_A_ADDR,
    BMX055_ACCEL_INT_B_ADDR,
    BMX055_ACCEL_INT_C_ADDR,
    BMX055_ACCEL_INT_D_ADDR,
    BMX055_ACCEL_FIFO_CONFIG_0_ADDR,
    BMX055_ACCEL_PMU_SELF_TEST_ADDR = 0x32,
    BMX055_ACCEL_TRIM_NVM_CTRL_ADDR,
    BMX055_ACCEL_BGW_SPI3_WDT_ADDR,
    BMX055_ACCEL_OFC_CTRL_ADDR = 0x36,
    BMX055_ACCEL_OFC_SETTING_ADDR,
    BMX055_ACCEL_OFC_OFFSET_X_ADDR,
    BMX055_ACCEL_OFC_OFFSET_Y_ADDR,
    BMX055_ACCEL_OFC_OFFSET_Z_ADDR,
    BMX055_ACCEL_TRIM_GP0_ADDR,
    BMX055_ACCEL_TRIM_GP1_ADDR,
    BMX055_ACCEL_FIFO_CONFIG_1_ADDR = 0x3E,
    BMX055_ACCEL_FIFO_DATA_ADDR
} BMX055AccelReg;

// GYRO REGISTER ADDRESSES
typedef enum {
    BMX055_GYRO_CHIPID_ADDR = 0x00,
    BMX055_GYRO_RATE_X_LSB_ADDR = 0x02,
    BMX055_GYRO_RATE_X_MSB_ADDR,
    BMX055_GYRO_RATE_Y_LSB_ADDR,
    BMX055_GYRO_RATE_Y_MSB_ADDR,
    BMX055_GYRO_RATE_Z_LSB_ADDR,
    BMX055_GYRO_RATE_Z_MSB_ADDR,
    BMX055_GYRO_INT_STATUS_0_ADDR = 0x09,
    BMX055_GYRO_INT_STATUS_1_ADDR,
    BMX055_GYRO_INT_STATUS_2_ADDR,
    BMX055_GYRO_INT_STATUS_3_ADDR,
    BMX055_GYRO_FIFO_STATUS_ADDR,
    BMX055_GYRO_RANGE_ADDR,
    BMX055_GYRO_BW_ADDR,
    BMX055_GYRO_LPM1_ADDR,
    BMX055_GYRO_LPM2_ADDR,
    BMX055_GYRO_RATE_HBW_ADDR,
    BMX055_GYRO_BGW_SOFTRESET_ADDR,
    BMX055_GYRO_INT_EN_0_ADDR,
    BMX055_GYRO_INT_EN_1_ADDR,
    BMX055_GYRO_INT_MAP_0_ADDR,
    BMX055_GYRO_INT_MAP_1_ADDR,
    BMX055_GYRO_INT_MAP_2_ADDR,
    BMX055_GYRO_1A_ADDR,
    BMX055_GYRO_1B_ADDR,
    BMX055_GYRO_1C_ADDR,
    BMX055_GYRO_1E_ADDR = 0x1E,
    BMX055_GYRO_INT_RST_LATCH_ADDR = 0x21,
    BMX055_GYRO_HIGH_TH_X_ADDR,
    BMX055_GYRO_HIGH_DUR_X_ADDR,
    BMX055_GYRO_HIGH_TH_Y_ADDR,
    BMX055_GYRO_HIGH_DUR_Y_ADDR,
    BMX055_GYRO_HIGH_TH_Z_ADDR,
    BMX055_GYRO_HIGH_DUR_Z_ADDR,
    BMX055_GYRO_SOC_ADDR = 0x31,
    BMX055_GYRO_A_FOC_ADDR,
    BMX055_GYRO_TRIM_NVM_CTRL_ADDR,
    BMX055_GYRO_BGW_SPI3_WDT_ADDR,
    BMX055_GYRO_OFC1_ADDR = 0x36,
    BMX055_GYRO_OFC2_ADDR,
    BMX055_GYRO_OFC3_ADDR,
    BMX055_GYRO_OFC4_ADDR,
    BMX055_GYRO_TRIM_GP0_ADDR,
    BMX055_GYRO_TRIM_GP1_ADDR,
    BMX055_GYRO_BIST_ADDR,
    BMX055_GYRO_FIFO_CONFIG_0_ADDR,
    BMX055_GYRO_FIFO_CONFIG_1_ADDR,
} BMX055GyroReg;

// MAGNET REGISTER ADDRESSES
typedef enum {
    BMX055_MAGNET_CHIPID_ADDR = 0x40,
    BMX055_MAGNET_DATA_X_LSB_ADDR = 0x42,
    BMX055_MAGNET_DATA_X_MSB_ADDR,
    BMX055_MAGNET_DATA_Y_LSB_ADDR,
    BMX055_MAGNET_DATA_Y_MSB_ADDR,
    BMX055_MAGNET_DATA_Z_LSB_ADDR,
    BMX055_MAGNET_DATA_Z_MSB_ADDR,
    BMX055_MAGNET_RHALL_LSB_ADDR,
    BMX055_MAGNET_RHALL_MSB_ADDR,
    BMX055_MAGNET_ISR_ADDR = 0x4A,
    BMX055_MAGNET_4B_ADDR,
    BMX055_MAGNET_4C_ADDR,
    BMX055_MAGNET_4D_ADDR,
    BMX055_MAGNET_4E_ADDR,
    BMX055_MAGNET_4F_ADDR,
    BMX055_MAGNET_50_ADDR,
    BMX055_MAGNET_51_ADDR,
    BMX055_MAGNET_52_ADDR,
} BMX055MagnetReg;

typedef union {
    uint8_t array[BMX055_ACCEL_DATA_SIZE];
    struct {
        uint16_t x;
        uint16_t y;
        uint16_t z;
        uint8_t temp;
    };
} BMX055_AccelData_t;

typedef union {
    uint8_t array[BMX055_GYRO_DATA_SIZE];
    struct {
        uint16_t x;
        uint16_t y;
        uint16_t z;
    };
} BMX055_GyroData_t;

typedef union {
    uint8_t array[BMX055_MAGNET_DATA_SIZE];
    struct {
        uint16_t x;
        uint16_t y;
        uint16_t z;
        uint16_t rhall;
    };
} BMX055_MagnetData_t;

void bmx055_setup(void);
void bmx055_on(void);
void bmx055_off(void);

/* =======================
 * ACCELEROMETER FUNCTIONS
 * =======================*/
/**
 * Reads the ID register from the accelerometer in the BMX055.
 *
 * @param buf Pointer to a one byte buffer.
 */
void bmx055_read_accel_whoami(uint8_t *buf);

/**
 * Reads all the data from the accelerometer in the BMX055.
 *
 * @param buf Pointer to the seven byte BMX055_AccelData_t union.
 */
void bmx055_accel_burst_read_data(BMX055_AccelData_t *buf);

/* ===================
 * GYROSCOPE FUNCTIONS
 * ===================*/
/**
 * Reads the ID register from the gyroscope in the BMX055.
 *
 * @param buf Pointer to a one byte buffer.
 */
void bmx055_read_gyro_whoami(uint8_t *buf);

/**
 * Reads all the data from the gyroscope in the BMX055.
 *
 * @param buf Pointer to a six byte buffer.
 */
void bmx055_gyro_burst_read_data(BMX055_GyroData_t *buf);

/* ======================
 * MAGNETOMETER FUNCTIONS
 * ======================*/
/**
 * Reads the ID register from the magnetometer in the BMX055.
 *
 * @param buf Pointer to a one byte buffer.
 */
void bmx055_read_magnet_whoami(uint8_t *buf);

/**
 * Setup the magnetometer for data reads.
 */
void bmx055_magnet_setup();

/**
 * Reads all the data from the magnetometer in the BMX055.
 *
 * @param buf Pointer to an eight byte buffer.
 */
void bmx055_magnet_burst_read_data(BMX055_MagnetData_t *buf);
