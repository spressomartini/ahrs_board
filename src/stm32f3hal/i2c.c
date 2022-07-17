#include <stdbool.h>
#include <stm32f302xc.h>
#include "stm32f3hal/rcc.h"
#include "stm32f3hal/i2c.h"

const I2C_Config i2c_100khz_config = {
    .base = I2C1,
    .prescaler = 0x0Fu,
    .sclh = 0x0Fu,
    .scll = 0x13u,
    .setup_time = 0x04u,
    .hold_time = 0x02u
};

const I2C_Config i2c_400khz_config = {
    .base = I2C1,
    .prescaler = 0x07u,
    .sclh = 0x03u,
    .scll = 0x09u,
    .setup_time = 0x03u,
    .hold_time = 0x03u
};

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

/************************ I2C CR1 ************************/
void i2c_enable(I2C_TypeDef *base) {
    // set I2C CR1 PE
    base->CR1 |= I2C_CR1_PE;
}

void i2c_disable(I2C_TypeDef *base) {
    // clear I2C CR1 PE
    base->CR1 &= ~I2C_CR1_PE;
}

/************************ I2C CR2 ************************/
void i2c_set_nbytes(I2C_TypeDef *base, uint8_t nbytes) {
    // read I2C CR2
    uint32_t reg = base->CR2;

    // clear bits [23:16] and set sadd
    reg &= ~I2C_CR2_NBYTES;
    reg |= (nbytes << I2C_CR2_NBYTES_Pos);

    // write into CR2
    base->CR2 = reg;
}

void i2c_master_autoend_on(I2C_TypeDef *base) {
    // set I2C CR2 AUTOEND
    base->CR2 |= I2C_CR2_AUTOEND;
}

void i2c_master_autoend_off(I2C_TypeDef *base) {
    // clear I2C CR2 AUTOEND
    base->CR2 &= ~I2C_CR2_AUTOEND;
}

void i2c_master_stop(I2C_TypeDef *base) {
    // set I2C CR2 STOP
    base->CR2 |= I2C_CR2_STOP;
}

void i2c_start_generation(I2C_TypeDef *base) {
    // set I2C CR2 START
    base->CR2 |= I2C_CR2_START;
}

void i2c_master_7b_addr_mode(I2C_TypeDef *base) {
    // clear I2C CR2 ADD10 for 7b
    base->CR2 &= ~I2C_CR2_ADD10;
}

void i2c_master_10b_addr_mode(I2C_TypeDef *base) {
    // set I2C CR2 ADD10 for 10b
    base->CR2 |= I2C_CR2_ADD10;
}

void i2c_master_write_transfer(I2C_TypeDef *base) {
    // clear I2C CR2 RD_WRN for write transfer
    base->CR2 &= ~I2C_CR2_RD_WRN;
}

void i2c_master_read_transfer(I2C_TypeDef *base) {
    // set I2C CR2 RD_WRN for read transfer
    base->CR2 |= I2C_CR2_RD_WRN;
}

void i2c_master_set_7b_slave_addr(I2C_TypeDef *base, uint8_t sadd) {
    // read I2C CR2
    uint32_t reg = base->CR2;

    // prepare sadd, we only care about the 7 LSB
    sadd = (sadd & 0x7F) << 1;

    // clear bits [9:0] and set sadd
    reg &= ~I2C_CR2_SADD;
    reg |= (sadd << I2C_CR2_SADD_Pos);

    // write into CR2
    base->CR2 = reg;
}

/************************ I2C TXDR ************************/
void i2c_put_data(I2C_TypeDef *base, uint8_t txdata) {
    // read I2C TXDR
    uint32_t reg = base->TXDR;

    // clear bits [7:0] and set txdata
    reg &= ~I2C_TXDR_TXDATA;
    reg |= (uint32_t)txdata;

    // write into TXDR
    base->TXDR = reg;
}

/************************ I2C RXDR ************************/
uint8_t i2c_get_data(I2C_TypeDef *base) {
    return base->RXDR & 0xFF;
}

/********************** I2C TIMINGR **********************/
void i2c_set_prescaler(I2C_TypeDef *base, uint8_t prescaler) {
    // fpre = fclk / (pre+1)

    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [31:28] and set prescaler
    reg &= ~I2C_TIMINGR_PRESC;
    reg |= (prescaler << I2C_TIMINGR_PRESC_Pos);

    // write into TIMINGR
    base->TIMINGR = reg;
}

void i2c_set_data_setup_time(I2C_TypeDef *base, uint8_t setup_time) {
    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [23:20] and set SCLDEL
    reg &= ~I2C_TIMINGR_SCLDEL;
    reg |= (setup_time << I2C_TIMINGR_SCLDEL_Pos);

    // write into TIMINGR
    base->TIMINGR = reg;
}

void i2c_set_data_hold_time(I2C_TypeDef *base, uint8_t hold_time) {
    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [19:16] and set SDADEL
    reg &= ~I2C_TIMINGR_SDADEL;
    reg |= (hold_time << I2C_TIMINGR_SDADEL_Pos);

    // write into TIMINGR
    base->TIMINGR = reg;
}

void i2c_master_set_scl_high_period(I2C_TypeDef *base, uint8_t sclh) {
    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [15:8] and set SCLH
    reg &= ~I2C_TIMINGR_SCLH;
    reg |= (sclh << I2C_TIMINGR_SCLH_Pos);

    // write into TIMINGR
    base->TIMINGR = reg;
}

void i2c_master_set_scl_low_period(I2C_TypeDef *base, uint8_t scll) {
    // read I2C TIMINGR
    uint32_t reg = base->TIMINGR;

    // clear bits [7:0] and set SCLL
    reg &= ~I2C_TIMINGR_SCLL;
    reg |= (scll << I2C_TIMINGR_SCLL_Pos);

    // write into TIMINGR
    base->TIMINGR = reg;
}

/********************** I2C TIMINGR **********************/
bool i2c_busy(I2C_TypeDef *base) {
    return base->ISR & I2C_ISR_BUSY;
}

bool i2c_arbitration_lost(I2C_TypeDef *base) {
    return base->ISR & I2C_ISR_ARLO;
}

bool i2c_transfer_complete(I2C_TypeDef *base) {
    return base->ISR & I2C_ISR_TC;
}

bool i2c_nack(I2C_TypeDef *base) {
    return base->ISR & I2C_ISR_NACKF;
}

bool i2c_received_data(I2C_TypeDef *base) {
    return base->ISR & I2C_ISR_RXNE;
}

bool i2c_tx_empty(I2C_TypeDef *base) {
    return base->ISR & I2C_ISR_TXE;
}

/********************* I2C FUNCTIONS *********************/
void i2c_master_setup(I2C_Config *conf) {
    // select the sysclk
    rcc_i2c_use_sysclk(conf->base);

    // enable the clock to the peripheral
    i2c_clock_enable(conf->base);

    // configure prescaler
    i2c_set_prescaler(conf->base, conf->prescaler);

    // configure sclh and scll
    i2c_master_set_scl_high_period(conf->base, conf->sclh);
    i2c_master_set_scl_low_period(conf->base, conf->scll);

    // configure setup and hold time
    i2c_set_data_setup_time(conf->base, conf->setup_time);
    i2c_set_data_hold_time(conf->base, conf->hold_time);

    // enable i2c
    i2c_enable(conf->base);
}

void i2c_master_transfer7(I2C_TypeDef *base, uint8_t addr, uint8_t *w, uint8_t nwrite, uint8_t *r, uint8_t nread) {
    bool wait;

    if (nwrite) {
        i2c_master_set_7b_slave_addr(base, addr);
        i2c_master_write_transfer(base);
        i2c_set_nbytes(base, nwrite);

        if (nread) {
            i2c_master_autoend_off(base);
        }
        else {
            i2c_master_autoend_on(base);
        }

        i2c_start_generation(base);

        while (nwrite--) {
            wait = true;
            while (wait) {
                // TODO make this a hal call
                if (base->ISR & I2C_ISR_TXIS) {
                    wait = false;
                }
                while (i2c_nack(base));
            }
            // send data
            i2c_put_data(base, *w++);
        }
        // FIXME may not be necessary
        if (nread) {
            while (!i2c_transfer_complete(base));
        }
    }

    if (nread) {
        i2c_master_set_7b_slave_addr(base, addr);
        i2c_master_read_transfer(base);
        i2c_set_nbytes(base, nread);

        i2c_start_generation(base);

        i2c_master_autoend_on(base);

        for (uint8_t i = 0; i < nread; i++) {
            while (!i2c_received_data(base));
            r[i] = i2c_get_data(base);
        }
    }
}
