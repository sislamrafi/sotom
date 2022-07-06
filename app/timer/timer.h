#ifndef __TIMER_H__
#define __TIMER_H__

#include "../registers/external_peripherals.h"

void ConfigTimer2ForDelay(void);
void Delay(uint32_t delay); // milisecond
void Delay_micro_second(uint32_t delay);

#endif