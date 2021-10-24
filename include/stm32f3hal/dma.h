#include <stm32f302xc.h>


typedef struct {
    DMA_Channel_TypeDef *base;
    IRQn_Type irqn;
    /* CPAR */
    uint32_t paddr;             // peripheral reg addr
    /* CMAR */
    uint32_t maddr;             // memory addr
    /* CNDTR */
    uint16_t ndata;             // size of data
    /* CCR */
    uint32_t pl;                // priority level
    uint32_t dir;               // data direction
    uint32_t circ;              // circular mode
    uint32_t minc;              // mem incr mode
    uint32_t pinc;              // peri incr mode
    bool ht_int;
    bool tc_int;
    bool int_pri;
} DMA_Config;

/* 
 * set peripheral register addr in DMA_CPARx
 * set memory address in DMA_CMARx
 * configure number of data to transfer in DMA_CNDTRx
 * configure parameters in DMA_CCRx: priority, data dir, circular mode,
 *     peri and mem increment mode, peri and mem data size, interrupt enable at 
 *     half, full, error
 * activate the channel by setting the EN bit in DMA_CCRx
 */

/* setup */
void dma_set_peripheral_addr(DMA_Channel_TypeDef *base, uint32_t addr);
void dma_set_memory_addr(DMA_Channel_TypeDef *base, uint32_t addr);
void dma_set_transfer_size(DMA_Channel_TypeDef *base, uint16_t num);
void dma_set_ccr(DMA_Channel_TypeDef *base, uint32_t pl, uint32_t dir,
                 uint32_t circ, uint32_t minc, uint32_t pinc);
void dma_channel_enable(DMA_Channel_TypeDef *base);
void dma_channel_disable(DMA_Channel_TypeDef *base);
void dma_setup(DMA_Config *conf);

/* interrupts */
void dma_register_irq(IRQn_Type irqn, uint32_t pri);
void dma_enable_tc_interrupt(DMA_Channel_TypeDef *base);
void dma_disable_tc_interrupt(DMA_Channel_TypeDef *base);
void dma_enable_ht_interrupt(DMA_Channel_TypeDef *base);
void dma_disable_ht_interrupt(DMA_Channel_TypeDef *base);
