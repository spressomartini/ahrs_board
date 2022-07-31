#pragma once
#include "stm32f3hal/rcc.h"
#include <stm32f302xc.h>

typedef struct {
    I2C_TypeDef *base;
    uint8_t prescaler;
    uint8_t sclh;
    uint8_t scll;
    uint8_t setup_time;  // scldel
    uint8_t hold_time;   // sdadel
} I2C_Config;

extern I2C_Config i2c_100khz_config;
extern I2C_Config i2c_400khz_config;

void i2c_clock_enable(I2C_TypeDef *base);

void i2c_enable(I2C_TypeDef *base);
void i2c_disable(I2C_TypeDef *base);

void i2c_set_nbytes(I2C_TypeDef *base, uint8_t nbytes);
void i2c_master_autoend_on(I2C_TypeDef *base);
void i2c_master_autoend_off(I2C_TypeDef *base);
void i2c_master_stop(I2C_TypeDef *base);
void i2c_start_generation(I2C_TypeDef *base);
void i2c_master_7b_addr_mode(I2C_TypeDef *base);
void i2c_master_10b_addr_mode(I2C_TypeDef *base);
void i2c_master_write_transfer(I2C_TypeDef *base);
void i2c_master_read_transfer(I2C_TypeDef *base);
void i2c_master_set_7b_slave_addr(I2C_TypeDef *base, uint8_t sadd);

void i2c_put_data(I2C_TypeDef *base, uint8_t txdata);
uint8_t i2c_get_data(I2C_TypeDef *base);

void i2c_set_prescaler(I2C_TypeDef *base, uint8_t prescaler);
void i2c_set_data_setup_time(I2C_TypeDef *base, uint8_t setup_time);
void i2c_set_data_hold_time(I2C_TypeDef *base, uint8_t hold_time);
void i2c_master_set_scl_high_period(I2C_TypeDef *base, uint8_t sclh);
void i2c_master_set_scl_low_period(I2C_TypeDef *base, uint8_t scll);

bool i2c_busy(I2C_TypeDef *base);
bool i2c_arbitration_lost(I2C_TypeDef *base);
bool i2c_transfer_complete(I2C_TypeDef *base);
bool i2c_nack(I2C_TypeDef *base);
bool i2c_received_data(I2C_TypeDef *base);
bool i2c_tx_empty(I2C_TypeDef *base);
void i2c_master_setup(I2C_Config *conf);

void i2c_master_transfer7(I2C_TypeDef *base, uint8_t addr, uint8_t *w, uint8_t nwrite, uint8_t *r, uint8_t nread);
