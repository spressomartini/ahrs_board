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

void dma_set_ccr(DMA_Channel_TypeDef *base, uint32_t pl, uint32_t msize,
        uint32_t psize, uint32_t minc, uint32_t pinc, uint32_t circ, uint32_t dir) {
    // read CCR
    uint32_t reg = base->CCR;

    reg &= ~DMA_CCR_PL;     // clear [13:12]
    reg &= ~DMA_CCR_MSIZE;  // clear [11:10]
    reg &= ~DMA_CCR_PSIZE;  // clear [9:8]
    reg &= ~DMA_CCR_MINC;   // clear [7]
    reg &= ~DMA_CCR_PINC;   // clear [6]
    reg &= ~DMA_CCR_CIRC;   // clear [5]
    reg &= ~DMA_CCR_DIR;    // clear [4]

    // modify
    reg |= pl;
    reg |= msize;
    reg |= psize;
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

void dma_clock_enable(DMA_TypeDef *base) {
    switch((uint32_t)base) {
        case (uint32_t)DMA1:
            RCC->AHBENR |= RCC_AHBENR_DMA1EN;
            break;
        case (uint32_t)DMA2:
            RCC->AHBENR |= RCC_AHBENR_DMA2EN;
            break;
    }
}

void dma_setup(DMA_Config *conf) {
    // enable the clocks
    dma_clock_enable(conf->dmabase);

    // disable the channel
    dma_channel_disable(conf->base);

    // configure registers
    dma_set_peripheral_addr(conf->base, conf->paddr);
    dma_set_memory_addr(conf->base, conf->maddr);
    dma_set_transfer_size(conf->base, conf->ndata);
    dma_set_ccr(conf->base, conf->pl, conf->msize, conf->psize, conf->minc,
            conf->pinc, conf->circ, conf->dir);

    // interrupts
    if (conf->tc_int) {
        dma_enable_tc_interrupt(conf->base);
    }
    if (conf->ht_int) {
        dma_enable_ht_interrupt(conf->base);
    }
    if (conf->te_int) {
        dma_enable_te_interrupt(conf->base);
    }

    // enable interrupts on the NVIC
    if (conf->tc_int || conf->ht_int || conf->te_int) {
        dma_register_irq(conf->irqn, conf->int_pri);
    }
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

void dma_enable_te_interrupt(DMA_Channel_TypeDef *base) {
    base->CCR |= DMA_CCR_TEIE;
}

void dma_disable_te_interrupt(DMA_Channel_TypeDef *base) {
    base->CCR &= ~DMA_CCR_TEIE;
}

/* INTERRUPT FLAGS */
void dma_clearflag_error(DMA_TypeDef *base, uint8_t channel) {
    base->IFCR |= DMA_IFCR_CTEIF(channel);
}

void dma_clearflag_ht(DMA_TypeDef *base, uint8_t channel) {
    base->IFCR |= DMA_IFCR_CHTIF(channel);
}

void dma_clearflag_tc(DMA_TypeDef *base, uint8_t channel) {
    base->IFCR |= DMA_IFCR_CTCIF(channel);
}

void dma_clearflag_global(DMA_TypeDef *base, uint8_t channel) {
    base->IFCR |= DMA_IFCR_CGIF(channel);
}
