#pragma once
#include "stm32f3hal/usart.h"
#include "stm32f3hal/gpio.h"
#include "stm32f3hal/rcc.h"
#include "drivers/uart.h"
#include "utils/queue.h"

#define UART1_RX_QUEUE_SIZE     (16u)
#define UART1_TX_QUEUE_SIZE     (16u)

/* setup */
void uart1_polling_setup(void);
void uart1_interrupt_setup(void);
void uart1_dma_setup(void);

/* dma usage */
void uart1_dma_transmit();

/* interrupt usage */
void uart1_interrupt_transmit(char *message, int size);
int uart1_interrupt_receive(char *buffer);
