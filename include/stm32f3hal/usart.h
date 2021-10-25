#pragma once
#include "stm32f3hal/rcc.h"
#include <stm32f302xc.h>
#include <stdbool.h>

/* defines */
#define USART_CR1_M0M1          ((0x1u << 28) | (0x1u << 12))
#define USART_7DATABITS         (0x1u << 28)
#define USART_8DATABITS         (0x0u)
#define USART_9DATABITS         (0x1u << 12)

/* structs */
typedef struct {
    USART_TypeDef *base;
    uint32_t baud;
    uint32_t usartclk;
    uint32_t clkf;
    uint32_t stopbits;
    uint32_t wordlen;
    uint32_t oversamp;
} USART_Config;

/* 
 * PROTOTYPES
 */
void usart_set_baud(USART_TypeDef *base, uint32_t freq, uint32_t baud);
void usart_set_stopbits(USART_TypeDef *base, uint32_t sbits);
void usart_set_oversampling(USART_TypeDef *base, uint32_t oversamp);
void usart_set_wordlen(USART_TypeDef *base, uint32_t wordlen);

void usart_enable(USART_TypeDef *base);
void usart_disable(USART_TypeDef *base);

void usart_tx_enable(USART_TypeDef *base);
void usart_tx_disable(USART_TypeDef *base);
void usart_rx_enable(USART_TypeDef *base);
void usart_rx_disable(USART_TypeDef *base);

void usart_clock_enable(USART_TypeDef *base);

void usart_setup(USART_Config *conf);

/* INTERRUPT */
void usart_register_irq(USART_TypeDef *base, uint32_t pri);
void usart_rx_interrupt_enable(USART_TypeDef *base);
void usart_rx_interrupt_disable(USART_TypeDef *base);
void usart_tx_interrupt_enable(USART_TypeDef *base);
void usart_tx_interrupt_disable(USART_TypeDef *base);
void usart_tc_interrupt_enable(USART_TypeDef *base);
void usart_tc_interrupt_disable(USART_TypeDef *base);

/* DMA */
void usart_dma_tx_enable(USART_TypeDef *base);
void usart_dma_tx_disable(USART_TypeDef *base);
void usart_dma_rx_enable(USART_TypeDef *base);
void usart_dma_rx_disable(USART_TypeDef *base);

/* STATUS */
bool usart_tdr_empty(USART_TypeDef *base);
bool usart_rdr_not_empty(USART_TypeDef *base);

/* FLAGS */
void usart_clearflag_tc(USART_TypeDef *base);
