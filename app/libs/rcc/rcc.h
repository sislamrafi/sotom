#ifndef __RCC_H__
#define __RCC_H__

#include "../../../core/stm32f446re/registers/external/peripherals.h"

const static uint32_t EXTERNAL_CLOCK_SPEED = 8000000U;

extern volatile uint32_t SYS_CLOCK_SPEED;

void rcc_config_HSE8MHz_SYS180MHz(void);

#endif
