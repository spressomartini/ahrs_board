#include "stm32f3hal/rcc.h"
#include "stm32f3hal/i2c.h"
#include <stm32f302xc.h>

typedef struct {
    I2C_TypeDef *base;
    uint32_t sclh;
    uint32_t scll;
    uint32_t prescaler;
    uint32_t setup_time;
    uint32_t hold_time;
} I2C_Config;

void i2c_enable(I2C_TypeDef *base) {
    // set I2C CR1 PE
    base->CR1 |= I2C_CR1_PE;
}

void i2c_disable(I2C_TypeDef *base) {
    // clear I2C CR1 PE
    base->CR1 &= ~I2C_CR1_PE;
}

void i2c_clock_enable(I2C_TypeDef *base) {
    switch((uint32_t)base) {
        case (uint32_t)I2C1:
            RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
            break;
        case (uint32_t)I2C2:
            RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
            break;
        default:
            break;
    }
}

void i2c_master_7b_addr_mode(I2C_TypeDef *base) {
    // clear I2C CR2 ADD10 for 7b
    base->CR2 &= ~I2C_CR2_ADD10;
}

void i2c_master_10b_addr_mode(I2C_TypeDef *base) {
    // set I2C CR2 ADD10 for 10b
    base->CR2 |= I2C_CR2_ADD10;
}

void i2c_set_prescaler(I2C_TypeDef *base, uint32_t prescaler) {
    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [31:28] and set prescaler
    reg &= ~I2C_TIMINGR_PRESC;
    reg |= prescaler;

    // write into TIMINGR
    base->TIMINGR = reg;
}

void i2c_set_data_setup_time(I2C_TypeDef *base, uint32_t setup_time) {
    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [23:20] and set SCLDEL
    reg &= ~I2C_TIMINGR_SCLDEL;
    reg |= setup_time;

    // write into TIMINGR
    base->TIMINGR = reg;
}

void i2c_set_data_hold_time(I2C_TypeDef *base, uint32_t hold_time) {
    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [19:16] and set SDADEL
    reg &= ~I2C_TIMINGR_SDADEL;
    reg |= hold_time;

    // write into TIMINGR
    base->TIMINGR = reg;
}

void i2c_master_set_scl_high_period(I2C_TypeDef *base, uint32_t sclh) {
    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [15:8] and set SCLH
    reg &= ~I2C_TIMINGR_SCLH;
    reg |= sclh;

    // write into TIMINGR
    base->TIMINGR = reg;
}

void i2c_master_set_scl_low_period(I2C_TypeDef *base, uint32_t scll) {
    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [7:0] and set SCLL
    reg &= ~I2C_TIMINGR_SCLL;
    reg |= scll;

    // write into TIMINGR
    base->TIMINGR = reg;
}

void i2c_master_setup(I2C_Config *conf) {
    // select the sysclk
    rcc_i2c_use_sysclk(conf->base);

    // enable the clock to the peripheral
    i2c_clock_enable(conf->base);

    // configure sclh and scll
    i2c_master_set_scl_high_period(conf->base, conf->sclh);
    i2c_master_set_scl_low_period(conf->base, conf->scll);

    // enable i2c
    i2c_enable(conf->base);
}
// 100khz
// fpre = fclk/(pre+1)
// presc = 0xf
// scll = 0x13
// sclh = 0xf
// sdadel = 0x2
// scldel = 0x4

// 400khz
// presc = 0x7
// scll = 0x9
// sclh = 0x3
// sdadel = 0x3
// scldel = 0x3
