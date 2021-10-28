#include "cm4/systick.h"

void systick_set_reload_val(uint32_t val) {
    SysTick->LOAD = (val & SysTick_LOAD_RELOAD_Msk);
}

int32_t systick_set_frequency(uint32_t f_hz) {
    uint32_t count = RCC_AHB_FREQUENCY / f_hz;
    int32_t errno = 0;

    // too big for even the div8 clock
    if (count >= SysTick_LOAD_RELOAD_Msk * 8) {
        errno = -1;
        goto exit;
    }
    // works for the div8 clock
    else if (count >= SysTick_LOAD_RELOAD_Msk) {
        count /= 8;
        SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
    }
    // works for the regular core clock
    else {
        SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    }

    // set reload val
    systick_set_reload_val(count - 1);
exit:
    return errno;
}

void systick_reset(void) {
    SysTick->VAL = 0;
}

void systick_enable_count(void) {
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void systick_disable_count(void) {
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void systick_setup(uint32_t f_hz, uint32_t gpri, uint32_t spri) {
    uint32_t pri;

    systick_reset();
    systick_set_frequency(f_hz);

    // enable interrupt
    systick_enable_interrupt();
    pri = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), gpri, spri);
    NVIC_SetPriority(SysTick_IRQn, pri);
    NVIC_EnableIRQ(SysTick_IRQn);

    systick_enable_count();
}

/* INTERRUPT */
void systick_enable_interrupt(void) {
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void systick_disable_interrupt(void) {
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}
