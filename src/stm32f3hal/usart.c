#include "stm32f3hal/rcc.h"
#include "stm32f3hal/usart.h"
#include <stm32f302xc.h>
#include <stdbool.h>

void usart_set_baud(USART_TypeDef *base, uint32_t freq, uint32_t baud) {
    // FIXME Assumes that oversampling is set to 16 in CR1_OVER8.
    // See Section 29.8.4 BRR[3:0] for more details.
    // Could limit the baud rate of the UART
    
    // set the baud rate (rounding up)
    base->BRR = (freq + baud / 2) / baud;
}

void usart_set_stopbits(USART_TypeDef *base, uint32_t sbits) {
    // read USART CR2
    uint32_t reg = base->CR2;

    // clear bits [13:12] and set STOP
    reg &= ~USART_CR2_STOP;
    reg |= sbits;

    // write into CR2
    base->CR2 = reg;
}

void usart_set_oversampling(USART_TypeDef *base, uint32_t oversamp) {
    // read USART CR1
    uint32_t reg = base->CR1;

    // clear bits [15] and set OVER8
    reg &= ~USART_CR1_OVER8;
    reg |= oversamp;

    // write into CR1
    base->CR1 = reg;
}

void usart_set_wordlen(USART_TypeDef *base, uint32_t wordlen) {
    // read USART CR1
    uint32_t reg = base->CR1;

    // clear bits [28,12] and set M0, M1
    reg &= ~USART_CR1_M0M1;
    reg |= wordlen;

    // write into CR1
    base->CR1 = reg;
}

void usart_enable(USART_TypeDef *base) {
    // set USART CR1 UE
    base->CR1 |= USART_CR1_UE;
}

void usart_disable(USART_TypeDef *base) {
    // clear USART CR1 UE
    base->CR1 &= ~USART_CR1_UE;
}

void usart_tx_enable(USART_TypeDef *base) {
    // set USART CR1 TE
    base->CR1 |= USART_CR1_TE;
}

void usart_tx_disable(USART_TypeDef *base) {
    // clear USART CR1 TE
    base->CR1 &= ~USART_CR1_TE;
}

void usart_rx_enable(USART_TypeDef *base) {
    // set USART CR1 RE
    base->CR1 |= USART_CR1_RE;
}

void usart_rx_disable(USART_TypeDef *base) {
    // clear USART CR1 RE
    base->CR1 &= ~USART_CR1_RE;
}

void usart_clock_enable(USART_TypeDef *base) {
    switch((uint32_t)base) {
        case (uint32_t)USART1:
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            break;
        case (uint32_t)USART2:
            RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
            break;
        case (uint32_t)USART3:
            RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
            break;
    }
}

void usart_setup(USART_Config *conf) {
    // TODO CLOCK SELECTION
    usart_clock_enable(conf->base);
    usart_disable(conf->base);
    usart_set_baud(conf->base, conf->clkf, conf->baud);
    usart_set_oversampling(conf->base, conf->oversamp);
    usart_set_wordlen(conf->base, conf->wordlen);
    usart_set_stopbits(conf->base, conf->stopbits);
    // TODO PARITY
    usart_tx_enable(conf->base);
    usart_rx_enable(conf->base);
    usart_enable(conf->base);
}

/* INTERRUPT */
void usart_register_irq(USART_TypeDef *base, uint32_t pri) {
    // select interrupt for USART
    IRQn_Type irqn;
    switch((uint32_t)base) {
        case (uint32_t)USART1:
            irqn = USART1_IRQn;
            break;
        case (uint32_t)USART2:
            irqn = USART2_IRQn;
            break;
        case (uint32_t)USART3:
            irqn = USART3_IRQn;
            break;
        default:
            // TODO raise some error
            return;
    }

    // register the interrupt with the NVIC
    NVIC_SetPriority(irqn, pri);
    NVIC_EnableIRQ(irqn);
}

void usart_rx_interrupt_enable(USART_TypeDef *base) {
    // set RXNEIE
    base->CR1 |= USART_CR1_RXNEIE;
}

void usart_rx_interrupt_disable(USART_TypeDef *base) {
    // clear RXNEIE
    base->CR1 &= ~USART_CR1_RXNEIE;
}

void usart_tx_interrupt_enable(USART_TypeDef *base) {
    // set TXEIE
    base->CR1 |= USART_CR1_TXEIE;
}

void usart_tx_interrupt_disable(USART_TypeDef *base) {
    // set TXEIE
    base->CR1 &= ~USART_CR1_TXEIE;
}

/* DMA */
void usart_dma_tx_enable(USART_TypeDef *base) {
    base->CR3 |= USART_CR3_DMAT;
}

void usart_dma_tx_disable(USART_TypeDef *base) {
    base->CR3 &= ~USART_CR3_DMAT;
}

void usart_dma_rx_enable(USART_TypeDef *base) {
    base->CR3 |= USART_CR3_DMAR;
}

void usart_dma_rx_disable(USART_TypeDef *base) {
    base->CR3 &= ~USART_CR3_DMAR;
}

/* STATUS */
bool usart_tdr_empty(USART_TypeDef *base) {
    return base->ISR & USART_ISR_TXE;
}

bool usart_rdr_not_empty(USART_TypeDef *base) {
    return base->ISR & USART_ISR_RXNE;
}
