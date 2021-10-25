#include <stm32f302xc.h>
#include <stdbool.h>

/* defines */
#define DMA_PL_LOW          (0u)
#define DMA_PL_MEDIUM       (DMA_CCR_PL_0)
#define DMA_PL_HIGH         (DMA_CCR_PL_1)
#define DMA_PL_VERYHIGH     (DMA_CCR_PL_0 | DMA_CCR_PL_1)

#define DMA_DIR_READ_PERIPH (0u)
#define DMA_DIR_READ_MEM    (DMA_CCR_DIR)

#define DMA_CIRC_DISABLE    (0u)
#define DMA_CIRC_ENABLE     (DMA_CCR_CIRC)

#define DMA_MINC_DISABLE    (0u)
#define DMA_MINC_ENABLE     (DMA_CCR_MINC)

#define DMA_PINC_DISABLE    (0u)
#define DMA_PINC_ENABLE     (DMA_CCR_PINC)

#define DMA_PSIZE_8BIT      (0u)
#define DMA_PSIZE_16BIT     (DMA_CCR_PSIZE_0)
#define DMA_PSIZE_32BIT     (DMA_CCR_PSIZE_1)

#define DMA_MSIZE_8BIT      (0u)
#define DMA_MSIZE_16BIT     (DMA_CCR_MSIZE_0)
#define DMA_MSIZE_32BIT     (DMA_CCR_MSIZE_1)

#define DMA_IFCR_CTEIF(x)   (0x8u << (((x) - 1) * 4))
#define DMA_IFCR_CHTIF(x)   (0x4u << (((x) - 1) * 4))
#define DMA_IFCR_CTCIF(x)   (0x2u << (((x) - 1) * 4))
#define DMA_IFCR_CGIF(x)    (0x1u << (((x) - 1) * 4))

/* structs */
typedef struct {
    DMA_TypeDef *dmabase;
    DMA_Channel_TypeDef *base;
    IRQn_Type irqn;
    /* CPAR */
    uint32_t paddr;    // peripheral reg addr
    /* CMAR */
    uint32_t maddr;    // memory addr
    /* CNDTR */
    uint16_t ndata;    // size of data
    /* CCR */
    uint32_t pl;       // priority level
    uint32_t dir;      // data direction
    uint32_t circ;     // circular mode
    uint32_t minc;     // mem incr mode
    uint32_t pinc;     // peri incr mode
    uint32_t psize;    // peripheral data size
    uint32_t msize;    // memory data size
    /* interrupts */
    bool ht_int;
    bool tc_int;
    bool te_int;
    uint32_t int_pri;
} DMA_Config;

/* setup */
void dma_set_peripheral_addr(DMA_Channel_TypeDef *base, uint32_t addr);
void dma_set_memory_addr(DMA_Channel_TypeDef *base, uint32_t addr);
void dma_set_transfer_size(DMA_Channel_TypeDef *base, uint16_t num);
void dma_set_ccr(DMA_Channel_TypeDef *base, uint32_t pl, uint32_t msize,
    uint32_t psize, uint32_t minc, uint32_t pinc, uint32_t circ, uint32_t dir);
void dma_channel_enable(DMA_Channel_TypeDef *base);
void dma_channel_disable(DMA_Channel_TypeDef *base);
void dma_clock_enable(DMA_TypeDef *base);
void dma_setup(DMA_Config *conf);

/* interrupts */
void dma_register_irq(IRQn_Type irqn, uint32_t pri);
void dma_enable_tc_interrupt(DMA_Channel_TypeDef *base);
void dma_disable_tc_interrupt(DMA_Channel_TypeDef *base);
void dma_enable_ht_interrupt(DMA_Channel_TypeDef *base);
void dma_disable_ht_interrupt(DMA_Channel_TypeDef *base);
void dma_enable_te_interrupt(DMA_Channel_TypeDef *base);
void dma_disable_te_interrupt(DMA_Channel_TypeDef *base);

/* interrupt flags */
void dma_clearflag_error(DMA_TypeDef *base, uint8_t channel);
void dma_clearflag_ht(DMA_TypeDef *base, uint8_t channel);
void dma_clearflag_tc(DMA_TypeDef *base, uint8_t channel);
void dma_clearflag_global(DMA_TypeDef *base, uint8_t channel);
