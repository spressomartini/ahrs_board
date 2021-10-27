//#include <core_cm4.h>
#include <stm32f302xc.h>
#include "stm32f3hal/rcc.h"

void systick_set_reload_val(uint32_t val);
int32_t systick_set_frequency(uint32_t f_hz);
void systick_reset(void);
void systick_enable_count(void);
void systick_disable_count(void);
void systick_setup(uint32_t f_hz, uint32_t gpri, uint32_t spri);
void systick_enable_interrupt(void);
void systick_disable_interrupt(void);
