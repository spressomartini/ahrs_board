#pragma once
#include "stm32f3hal/usart.h"
#include "stm32f3hal/gpio.h"
#include "stm32f3hal/rcc.h"
#include "drivers/uart.h"
#include "utils/queue.h"

#define UART1_TX_QUEUE_SIZE     (1024u)
#define UART1_MAX_TX_SIZE       (32u)

#define UART1_DMA_CHANNEL       (4u)

/* state machine enums */
typedef enum {
    IDLE,
    SEND,
    BUSY,
    DONE,
    ERROR
} uart_dma_state_t;

typedef enum {
    NONE,
    DMA_COMPLETE,
    UART_FREE,
    DMA_ERROR
} uart_dma_input_t;

/* setup */
void uart1_polling_setup(void);
void uart1_dma_setup(void);

/* dma usage */
int32_t uart1_queue_transmit(const char *str, size_t len);
void uart1_dma_fsm(void);
