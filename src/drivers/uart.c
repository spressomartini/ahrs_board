#include "stm32f3hal/usart.h"
#include "stm32f3hal/gpio.h"
#include "stm32f3hal/rcc.h"
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

/* UART1 QUEUE */
static queue_t uart1_rx_queue;
static queue_t uart1_tx_queue;
static char uart1_rx_buffer[UART1_RX_QUEUE_SIZE];
static char uart1_tx_buffer[UART1_TX_QUEUE_SIZE];

/* SETUP FUNCTIONS */
void uart1_setup(void) {
    gpio_pin_setup(&usart1tx_pin_config);
    gpio_pin_setup(&usart1rx_pin_config);
    usart_setup(&usart1_config);
}

void uart1_interrupt_setup(void) {
    /* setup uart1 */
    gpio_pin_setup(&usart1tx_pin_config);
    gpio_pin_setup(&usart1rx_pin_config);
    usart_setup(&usart1_config);

    /* setup uart1 software queues */
    queue_init(&uart1_rx_queue, uart1_rx_buffer, UART1_RX_QUEUE_SIZE);
    queue_init(&uart1_tx_queue, uart1_tx_buffer, UART1_TX_QUEUE_SIZE);

    // enable rx interrupt on the peripheral
    usart_rx_interrupt_enable(USART1);

    // enable interrupts at the NVIC
    // FIXME: CHECK PRIORITY
    usart_register_irq(USART1, 1);
}

/* USAGE FUNCTIONS */
void usart1_interrupt_transmit(char *message, int size) {
    // enqueue the message
    // FIXME: doesn't check if push successful
    for (int i = 0; i < size; i++) {
        queue_push(&uart1_tx_queue, message[i]);
    }
    
    // enable the transmit interrupt
    usart_tx_interrupt_enable(USART1);
}

int usart1_interrupt_receive(char *buffer) {
    int count = 0;
    char c;
    while (1) {
        c = queue_pop(&uart1_rx_queue);
        if (uart1_rx_queue.op_ok) {
            *buffer++ = c;
            count++;
        }
        else {
            break;
        }
    }
    return count;
}

/* INTERRUPT HANDLER */
void USART1_IRQHandler() {
    char c;

    // toggle led if interrupted
    led_toggle(RED_LED_PIN);

    // recieve data register not empty
    if (USART1->ISR & USART_ISR_RXNE) {
        c = USART1->RDR & 0xFF;
        queue_push(&uart1_rx_queue, c);
    }
    // transmit data register empty
    else if (USART1->ISR & USART_ISR_TXE) {
        c = queue_pop(&uart1_tx_queue);
        if (uart1_tx_queue.op_ok) {
            USART1->TDR = c;
        }
        // nothing left to send, disable the int
        else {
            usart_tx_interrupt_disable(USART1);
        }
    }
}
