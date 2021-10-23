#pragma once
#include <stm32f302xc.h>

/* DEFINES */
#define GPIO_MODER_MASK_PIN(n)      (0x3u << (2 * n))
#define GPIO_MODER_SHIFT(n, mode)   (mode << (2 * n))
#define GPIO_MODER_INPUT            (0x0u)
#define GPIO_MODER_OUTPUT           (0x1u)
#define GPIO_MODER_ALT              (0x2u)
#define GPIO_MODER_ANALOG           (0x3u)

#define GPIO_PUPDR_MASK_PIN(n)      (0x3u << (2 * n))
#define GPIO_PUPDR_SHIFT(n, pupd)   (pupd << (2 * n))
#define GPIO_PUPDR_NOPULL           (0x0u)
#define GPIO_PUPDR_PULLUP           (0x1u)
#define GPIO_PUPDR_PULLDOWN         (0x2u)

#define GPIO_OTYPER_MASK_PIN(n)     (0x1u << n)
#define GPIO_OTYPER_SHIFT(n, otype) (otype << n)
#define GPIO_OTYPER_PUSHPULL        (0x0u)
#define GPIO_OTYPER_OPENDRAIN       (0x1u)

#define GPIO_OSPEEDR_MASK_PIN(n)    (0x3u << (2 * n))
#define GPIO_OSPEEDR_SHIFT(n, ospeed) (ospeed << (2 * n))
#define GPIO_OSPEEDR_LOW            (0x0u)
#define GPIO_OSPEEDR_MEDIUM         (0x1u)
#define GPIO_OSPEEDR_HIGH           (0x3u)

#define GPIO_AFR_MASK_PIN(n)        (0xF << (4 * n))
#define GPIO_AFR_SHIFT(n, alt)      (alt << (4 * n))

/* STRUCTS */
typedef struct{
    GPIO_TypeDef *base;
    uint32_t pin;
    uint32_t mode;
    uint32_t pull;
    uint32_t otype;
    uint32_t ospeed;
    uint32_t alt;
} GPIO_PIN_Config;

typedef enum {
    GPIO_PIN_RESET = 0u,
    GPIO_PIN_SET
} GPIO_PIN_State;

/* FUNCTIONS */
void gpio_pin_set_mode(GPIO_TypeDef *base, uint32_t pin, uint32_t mode);
void gpio_pin_set_pull(GPIO_TypeDef *base, uint32_t pin, uint32_t pull);
void gpio_pin_set_otype(GPIO_TypeDef *base, uint32_t pin, uint32_t otype);
void gpio_pin_set_ospeed(GPIO_TypeDef *base, uint32_t pin, uint32_t ospeed);
void gpio_pin_set_alt(GPIO_TypeDef *base, uint32_t pin, uint32_t alt);
void gpio_pin_setup(GPIO_PIN_Config *pin_conf);
void gpio_write_pin(GPIO_TypeDef *base, uint32_t pin, GPIO_PIN_State state);
GPIO_PIN_State gpio_read_pin_output(GPIO_TypeDef *base, uint32_t pin);
void gpio_clock_enable(GPIO_TypeDef *base);
