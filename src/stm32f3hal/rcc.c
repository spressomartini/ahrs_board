#include "stm32f3hal/rcc.h"
#include "stm32f3hal/flash.h"
#include <stm32f302xc.h>
#include <stdbool.h>

/* CONFIG STRUCT */
const RCC_CLK_Config rcc_default_config = {
    .pllsrc = RCC_CFGR_PLLSRC_HSE_PREDIV,
    .plldiv = RCC_CFGR2_PREDIV_DIV1,
    .pllmul = RCC_CFGR_PLLMUL4,
    .sysclksrc = RCC_CFGR_SW_PLL,
    .sysclksws = RCC_CFGR_SWS_PLL,
    .flashws = FLASH_ACR_LATENCY_2,
    .hpre = RCC_CFGR_HPRE_DIV1,
    .ppre1 = RCC_CFGR_PPRE1_DIV2,
    .ppre2 = RCC_CFGR_PPRE2_DIV1,
    .ahb_f = 64000000u,
    .apb1_f = 32000000u,
    .apb2_f = 64000000u
};

/* GLOBAL CLOCK VARIABLES */
uint32_t RCC_AHB_FREQUENCY = 0u;
uint32_t RCC_APB1_FREQUENCY = 0u;
uint32_t RCC_APB2_FREQUENCY = 0u;

/* HSE CODE */
void rcc_hse_on(void) {
    RCC->CR |= RCC_CR_HSEON;
}

void rcc_hse_off(void) {
    RCC->CR &= ~RCC_CR_HSEON;
}

bool rcc_hse_rdy(void) {
    return RCC->CR & RCC_CR_HSERDY;
}

void rcc_hse_wait_for_rdy(void) {
    while(!rcc_hse_rdy());
}

/* PLL CODE */
bool rcc_pll_rdy(void) {
    return RCC->CR & RCC_CR_PLLRDY;
}

void rcc_pll_wait_for_rdy(void) {
    while(!rcc_pll_rdy());
}

void rcc_pll_wait_for_notrdy(void) {
    while(rcc_pll_rdy());
}

void rcc_pll_on(void) {
    RCC->CR |= RCC_CR_PLLON;
}

void rcc_pll_off(void) {
    RCC->CR &= ~RCC_CR_PLLON;
}

void rcc_pll_set_src(uint32_t pllsrc) {
    // read RCC CFGR
    uint32_t reg = RCC->CFGR;

    // clear [16] and set PLLSRC
    reg &= ~RCC_CFGR_PLLSRC;
    reg |= pllsrc;

    // write into CFGR
    RCC->CFGR = reg;
}

void rcc_pll_set_multiplier(uint32_t pllmul) {
    // read RCC CFGR
    uint32_t reg = RCC->CFGR;

    // clear [21:18] and set PLLMUL
    reg &= ~RCC_CFGR_PLLMUL;
    reg |= pllmul;

    // write into CFGR
    RCC->CFGR = reg;
}

void rcc_pll_set_prediv(uint32_t prediv) {
    // read RCC CFGR2
    uint32_t reg = RCC->CFGR2;

    // clear [3:0] and set PREDIV
    reg &= ~RCC_CFGR2_PREDIV;
    reg |= prediv;

    // write into CFGR2
    RCC->CFGR2 = reg;
}

/* SYSCLK CODE */
void rcc_sysclk_set_src(uint32_t src) {
    // read RCC CFGR
    uint32_t reg = RCC->CFGR;

    // clear [1:0] and set SW
    reg &= ~RCC_CFGR_SW;
    reg |= src;

    // write into CFGR
    RCC->CFGR = reg;
}

void rcc_sysclk_wait_for_sws(uint32_t sws) {
    while((RCC->CFGR & RCC_CFGR_SWS) != sws);
}

/* PRESCALAR */
void rcc_ahb_set_pre(uint32_t hpre) {
    // read RCC CFGR
    uint32_t reg = RCC->CFGR;

    // clear [7:4] and set HPRE
    reg &= ~RCC_CFGR_HPRE;
    reg |= hpre;

    // write into CFGR
    RCC->CFGR = reg;
}

void rcc_apb1_set_pre(uint32_t ppre1) {
    // read RCC CFGR
    uint32_t reg = RCC->CFGR;

    // clear [10:8] and sest PPRE1
    reg &= ~RCC_CFGR_PPRE1;
    reg |= ppre1;

    // write into CFGR
    RCC->CFGR = reg;
}

void rcc_apb2_set_pre(uint32_t ppre2) {
    // read RCC CFGR
    uint32_t reg = RCC->CFGR;

    // clear [13:11] and set PPRE2
    reg &= ~RCC_CFGR_PPRE2;
    reg |= ppre2;

    // write into CFGR
    RCC->CFGR = reg;
}

/* CLOCK SETUP */
void rcc_clock_hse_pll_setup(const RCC_CLK_Config *clk_conf) {
    // turn on the HSE and wait until ready
    rcc_hse_on();
    rcc_hse_wait_for_rdy();
    
    // turn off the PLL, configure, turn on PLL
    rcc_pll_off();
    rcc_pll_wait_for_notrdy();
    rcc_pll_set_src(clk_conf->pllsrc);
    rcc_pll_set_multiplier(clk_conf->pllmul);
    rcc_pll_set_prediv(clk_conf->plldiv);
    rcc_pll_on();
    rcc_pll_wait_for_rdy();

    // configure flash prefetching/waitstate (Section 4.2.2, Page 64)
    flash_enable_prefetch();
    flash_set_latency(clk_conf->flashws);
    
    // set the AHB, APB1, APB2 prescalars
    rcc_ahb_set_pre(clk_conf->hpre);
    rcc_apb1_set_pre(clk_conf->ppre1);
    rcc_apb2_set_pre(clk_conf->ppre2);
    
    // set PLL as sysclk source and wait until ready
    rcc_sysclk_set_src(clk_conf->sysclksrc);
    rcc_sysclk_wait_for_sws(clk_conf->sysclksws);

    // save the peripheral clock frequencies
    RCC_AHB_FREQUENCY = clk_conf->ahb_f;
    RCC_APB1_FREQUENCY = clk_conf->apb1_f;
    RCC_APB2_FREQUENCY = clk_conf->apb2_f;
}

/* PERIPHERAL CLOCKING */
void rcc_i2c_use_sysclk(I2C_TypeDef *base) {
    if(base == I2C1) {
        RCC->CFGR3 |= RCC_CFGR3_I2C1SW;
    }
    if(base == I2C2) {
        RCC->CFGR3 |= RCC_CFGR3_I2C2SW;
    }
}

void rcc_i2c_use_hsi(I2C_TypeDef *base) {
    if(base == I2C1) {
        RCC->CFGR3 &= ~RCC_CFGR3_I2C1SW;
    }
    if(base == I2C2) {
        RCC->CFGR3 &= ~RCC_CFGR3_I2C2SW;
    }
}
