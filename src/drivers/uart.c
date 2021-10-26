#include "stm32f3hal/usart.h"
#include "stm32f3hal/gpio.h"
#include "stm32f3hal/rcc.h"
#include "stm32f3hal/dma.h"
#include "utils/queue.h"
#include "drivers/uart.h"
#include "drivers/leds.h"

/* UART1 CONFIGS */
static GPIO_PIN_Config usart1tx_pin_config = {
    .base = GPIOA,
    .pin = 9,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 7,
};

static GPIO_PIN_Config usart1rx_pin_config = {
    .base = GPIOA,
    .pin = 10,
    .mode = GPIO_MODER_ALT,
    .pull = GPIO_PUPDR_NOPULL,
    .otype = GPIO_OTYPER_PUSHPULL,
    .ospeed = GPIO_OSPEEDR_HIGH,
    .alt = 7,
};

static USART_Config usart1_config = {
    .base = USART1,
    .baud = 115200,
    .clkf = 64000000,
    .stopbits = 0,
    .wordlen = USART_8DATABITS,
};

static DMA_Config dma1ch4_config = {
    .dmabase = DMA1,
    .base = DMA1_Channel4,
    .irqn = DMA1_Channel4_IRQn,
    .paddr = (uint32_t)&USART1->TDR,
    .maddr = 0,     // to be set later bc mem->periph
    .ndata = 0,     // to be set later
    .pl = DMA_PL_HIGH,
    .dir = DMA_DIR_READ_MEM,
    .circ = DMA_CIRC_DISABLE,
    .minc = DMA_MINC_ENABLE,
    .pinc = DMA_PINC_DISABLE,
    .psize = DMA_PSIZE_8BIT,
    .msize = DMA_MSIZE_8BIT,
    .ht_int = false,
    .tc_int = true,
    .te_int = true,
    .int_gpri = 7u,
    .int_spri = 0u,
};

/* UART1 QUEUE */
static queue_t uart1_tx_queue;
static char uart1_tx_buffer[UART1_TX_QUEUE_SIZE];

/* STATE MACHINE GLOBALS */
static uart_dma_state_t uart1_state = IDLE;
static volatile uart_dma_input_t uart1_sm_input;
static size_t uart1_dma_tx_blocksize;

/* SETUP FUNCTIONS */
void uart1_polling_setup(void) {
    /* setup uart1 */
    gpio_pin_setup(&usart1tx_pin_config);
    gpio_pin_setup(&usart1rx_pin_config);
    usart_setup(&usart1_config);
}

void uart1_dma_setup(void) {
    /* setup uart1 */
    gpio_pin_setup(&usart1tx_pin_config);
    gpio_pin_setup(&usart1rx_pin_config);
    usart_setup(&usart1_config);

    /* enable tc interrupt on uart1 */
    usart_tc_interrupt_enable(USART1);
    usart_tx_interrupt_disable(USART1);

    // enable interrupts at the NVIC
    usart_register_irq(USART1, 15u, 0u);

    /* enable dma tx on uart1 */
    usart_dma_tx_enable(USART1);

    /* setup dma1ch4 */
    dma_setup(&dma1ch4_config);

    /* init queue */
    queue_init(&uart1_tx_queue, uart1_tx_buffer, UART1_TX_QUEUE_SIZE);

    /* init state machine */
    uart1_state = IDLE;
    uart1_sm_input = NONE;
    uart1_dma_tx_blocksize = 0;
}

/* DMA USAGE FUNCTIONS */
uint32_t uart1_queue_transmit(const char *str, size_t len) {
    if (uart1_tx_queue.cap - uart1_tx_queue.size >= len) {
        for (size_t i = 0; i < len; i++) {
            queue_push(&uart1_tx_queue, *str++);
        }
    }
    return -1;
}

/* DMA TX STATE MACHINE */
void uart1_dma_fsm(void) {
    uart_dma_state_t next_state = uart1_state;
    char *block_addr;

    switch(uart1_state) {
        case IDLE:
            // only transition if queue is not empty
            if (uart1_tx_queue.size > 0) {
                next_state = SEND;
            }
        break;
        case SEND:
            // prepare the queue
            uart1_dma_tx_blocksize = queue_block_read_len(&uart1_tx_queue);
            if (uart1_dma_tx_blocksize > UART1_MAX_TX_SIZE) {
                uart1_dma_tx_blocksize = UART1_MAX_TX_SIZE;
            }
            block_addr = queue_block_read_addr(&uart1_tx_queue);

            // init dma transfer
            dma_set_transfer_size(dma1ch4_config.base, uart1_dma_tx_blocksize);
            dma_set_memory_addr(dma1ch4_config.base, (uint32_t)block_addr);
            dma_clearflag_global(DMA1, UART1_DMA_CHANNEL);
            dma_channel_enable(dma1ch4_config.base);

            // state transition
            next_state = BUSY;
        break;
        case BUSY:
            if (uart1_sm_input == DMA_ERROR) {
                next_state = ERROR;
            }
            else if (uart1_sm_input == DMA_COMPLETE || uart1_sm_input == UART_FREE) {
                next_state = DONE;
            }
        break;
        case DONE:
            // turn off dma
            dma_channel_disable(dma1ch4_config.base);

            // update the queue
            queue_skip(&uart1_tx_queue, uart1_dma_tx_blocksize);

            // state transition
            if (uart1_sm_input == UART_FREE) {
                next_state = IDLE;
            }
        break;
        case ERROR:
            led_off(RED_LED_PIN);
            led_off(GREEN_LED_PIN);
        break;
    }
    uart1_state = next_state;
}

/* INTERRUPT HANDLERS */
void DMA1_Channel4_IRQHandler(void) {
    // transfer complete interrupt
    if (DMA1->ISR & DMA_ISR_TCIF4) {
        dma_clearflag_tc(DMA1, UART1_DMA_CHANNEL);
        uart1_sm_input = DMA_COMPLETE;
    }
    // transfer error interrupt
    else if (DMA1->ISR & DMA_ISR_TEIF4) {
        dma_clearflag_error(DMA1, UART1_DMA_CHANNEL);
        uart1_sm_input = DMA_ERROR;
    }
}

void USART1_IRQHandler(void) {
    if (USART1->ISR & USART_ISR_TC) {
        usart_clearflag_tc(USART1);
        uart1_sm_input = UART_FREE;
    }
}
