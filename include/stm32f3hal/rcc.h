#pragma once
#include <stm32f302xc.h>
#include <stdbool.h>

typedef struct {
   uint32_t pllsrc;
   uint32_t pllmul;
   uint32_t plldiv;
   uint32_t sysclksrc;
   uint32_t sysclksws;
   uint32_t flashws;
   uint32_t hpre;
   uint32_t ppre1;
   uint32_t ppre2;
   uint32_t ahb_f;
   uint32_t apb1_f;
   uint32_t apb2_f;
} RCC_CLK_Config;

extern const RCC_CLK_Config rcc_default_config;
extern uint32_t RCC_AHB_FREQUENCY;
extern uint32_t RCC_APB1_FREQUENCY;
extern uint32_t RCC_APB2_FREQUENCY;

void rcc_hse_on(void);
void rcc_hse_off(void);
bool rcc_hse_rdy(void);
void rcc_hse_wait_for_rdy(void);

bool rcc_pll_rdy(void);
void rcc_pll_wait_for_rdy(void);
void rcc_pll_wait_for_notrdy(void);
void rcc_pll_on(void);
void rcc_pll_off(void);
void rcc_pll_set_src(uint32_t pllsrc);
void rcc_pll_set_multiplier(uint32_t pllmul);
void rcc_pll_set_prediv(uint32_t prediv);

void rcc_sysclk_set_src(uint32_t src);
void rcc_sysclk_wait_for_sws(uint32_t sws);

void rcc_ahb_set_pre(uint32_t hpre);
void rcc_apb1_set_pre(uint32_t ppre1);
void rcc_apb2_set_pre(uint32_t ppre2);

void rcc_clock_hse_pll_setup(const RCC_CLK_Config *clk_conf);

/* PERIPHERAL CLOCKING */
void rcc_i2c_use_sysclk(I2C_TypeDef *base);
void rcc_i2c_use_hsi(I2C_TypeDef *base);
