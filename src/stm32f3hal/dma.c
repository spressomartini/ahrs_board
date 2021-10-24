#include <stm32f302xc.h>
#include "stm32f3hal/dma.h"


void dma_set_peripheral_addr(DMA_Channel_TypeDef *base, uint32_t addr) {
    base->CPAR = addr;
}

void dma_set_memory_addr(DMA_Channel_TypeDef *base, uint32_t addr) {
    base->CMAR = addr;
}

void dma_set_transfer_size(DMA_Channel_TypeDef *base, uint16_t num) {
    base->CNDTR = num;
}

void dma_set_ccr(DMA_Channel_TypeDef *base, uint32_t pl, uint32_t dir,
                 uint32_t circ, uint32_t minc, uint32_t pinc) {
    uint32_t reg = base->CCR;

    reg &= ~DMA_CCR_PL;     // clear [13:12]
    reg &= ~DMA_CCR_MINC;   // clear [7]
    reg &= ~DMA_CCR_PINC;   // clear [6]
    reg &= ~DMA_CCR_CIRC;   // clear [5]
    reg &= ~DMA_CCR_DIR;    // clear [4]

    // modify
    reg |= pl;
    reg |= minc;
    reg |= pinc;
    reg |= circ;
    reg |= dir;

    // write in the changes
    base->CCR = reg;
}

void dma_channel_enable(DMA_Channel_TypeDef *base) {
    base->CCR |= DMA_CCR_EN;
}

void dma_channel_disable(DMA_Channel_TypeDef *base) {
    base->CCR &= ~DMA_CCR_EN;
}

void dma_setup(DMA_Config *conf) {
    // disable the channel
    dma_channel_disable(conf->base);

    // configure registers
    dma_set_peripheral_addr(conf->base, conf->paddr);
    dma_set_memory_addr(conf->base, conf->maddr);
    dma_set_transfer_size(conf->base, conf->ndata);
    dma_set_ccr(conf->base, conf->pl, conf->dir, conf->circ, conf->minc, conf->pinc);

    // interrupts
    if (conf->tc_int) {
        dma_enable_tc_interrupt(conf->base);
    }
    if (conf->ht_int) {
        dma_enable_ht_interrupt(conf->base);
    }

    // enable interrupts on the NVIC
    if (conf->tc_int || conf->ht_int) {
        dma_register_irq(conf->irqn, conf->int_pri);
    }

    // enable the channel
    dma_channel_enable(conf->base);
}

/* INTERRUPTS */
void dma_register_irq(IRQn_Type irqn, uint32_t pri) {
    // register the interrupt with the NVIC
    NVIC_SetPriority(irqn, pri);
    NVIC_EnableIRQ(irqn);
}

void dma_enable_tc_interrupt(DMA_Channel_TypeDef *base) {
    base->CCR |= DMA_CCR_TCIE;
}

void dma_disable_tc_interrupt(DMA_Channel_TypeDef *base) {
    base->CCR &= ~DMA_CCR_TCIE;
}

void dma_enable_ht_interrupt(DMA_Channel_TypeDef *base) {
    base->CCR |= DMA_CCR_HTIE;
}

void dma_disable_ht_interrupt(DMA_Channel_TypeDef *base) {
    base->CCR &= ~DMA_CCR_HTIE;
}
