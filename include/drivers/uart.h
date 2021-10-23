#pragma once
#include "stm32f3hal/usart.h"
#include "stm32f3hal/gpio.h"
#include "stm32f3hal/rcc.h"
#include "drivers/uart.h"
#include "utils/queue.h"

#define UART1_RX_QUEUE_SIZE     (16u)
#define UART1_TX_QUEUE_SIZE     (16u)

void uart1_setup(void);
void uart1_interrupt_setup(void);
void usart1_interrupt_transmit(char *message, int size);
int usart1_interrupt_receive(char *buffer);
