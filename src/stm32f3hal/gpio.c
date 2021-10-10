#include "stm32f3hal/gpio.h"
#include <stm32f302xc.h>

void gpio_pin_set_mode(GPIO_TypeDef *base, uint32_t pin, uint32_t mode) {
    // clear the pin mode
    base->MODER &= ~GPIO_MODER_MASK_PIN(pin);

    // set the mode
    base->MODER |= GPIO_MODER_SHIFT(pin, mode);
}

void gpio_pin_set_pull(GPIO_TypeDef *base, uint32_t pin, uint32_t pull) {
    // clear the pin pull direction
    base->PUPDR &= ~GPIO_PUPDR_MASK_PIN(pin);

    // set the pull direction
    base->PUPDR |= GPIO_PUPDR_SHIFT(pin, pull);
}

void gpio_pin_set_otype(GPIO_TypeDef *base, uint32_t pin, uint32_t otype) {
    // clear the pin otype
    base->OTYPER &= ~GPIO_OTYPER_MASK_PIN(pin);

    // set the otype
    base->OTYPER |= GPIO_OTYPER_SHIFT(pin, otype);
}

void gpio_pin_set_ospeed(GPIO_TypeDef *base, uint32_t pin, uint32_t ospeed) {
    // clear the pin speed
    base->OSPEEDR &= ~GPIO_OSPEEDR_MASK_PIN(pin);

    // set the speed
    base->OSPEEDR |= GPIO_OSPEEDR_SHIFT(pin, ospeed);
}

void gpio_pin_set_alt(GPIO_TypeDef *base, uint32_t pin, uint32_t alt) {
    // determine which register to write to
    if(pin > 7) {
        // correct shift for new register
        pin -= 8;

        // clear the pin alt
        base->AFR[1] &= ~GPIO_AFR_MASK_PIN(pin);

        // set the alt
        base->AFR[1] |= GPIO_AFR_SHIFT(pin, alt);
    }
    else {
        // clear the pin alt
        base->AFR[0] &= ~GPIO_AFR_MASK_PIN(pin);

        // set the alt
        base->AFR[0] |= GPIO_AFR_SHIFT(pin, alt);
    }
}

void gpio_pin_setup(GPIO_PIN_Config *pin_conf) {
    gpio_clock_enable(pin_conf->base);
    gpio_pin_set_mode(pin_conf->base, pin_conf->pin, pin_conf->mode);
    gpio_pin_set_pull(pin_conf->base, pin_conf->pin, pin_conf->pull);
    gpio_pin_set_otype(pin_conf->base, pin_conf->pin, pin_conf->otype);
    gpio_pin_set_ospeed(pin_conf->base, pin_conf->pin, pin_conf->ospeed);
    if(pin_conf->mode == GPIO_MODER_ALT) {
        gpio_pin_set_alt(pin_conf->base, pin_conf->pin, pin_conf->alt);
    }
}

void gpio_write_pin(GPIO_TypeDef *base, uint32_t pin, GPIO_PIN_State state) {
    if(state != GPIO_PIN_RESET) {
        base->BSRR = (1 << pin);
    }
    else {
        base->BRR = (1 << pin);
    }
}

void gpio_clock_enable(GPIO_TypeDef *base) {
    switch((uint32_t)base) {
        case (uint32_t)GPIOA:
            RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
            break;
        case (uint32_t)GPIOB:
            RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
            break;
    }

    // read the AHBENR
    RCC->AHBENR;
}
