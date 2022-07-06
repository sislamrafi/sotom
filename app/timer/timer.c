#include "timer.h"

void ConfigTimer2ForDelay(void) {
  // 1. Enable Timer2 clock
  RCC->APB1ENR |= 1 << 0;

  // 2. Set the Prescalar
  TIM2->PSC = 90 - 1;

  /*
          THE APB1 BUS CLOCK IS 90MHz
          SO EVERY TIMER DELAY IS 90MHz/90 = 1 microseconds
  */

  // 3. Set the Auto-Reload-Resister value. Set max value for 32 bits.
  TIM2->ARR = 0xffff - 1;

  // 4. Enable the Timer, and wait for the update Flag to set
  TIM2->CR1 |= 1 << 0;
  while (!(TIM2->SR & (1 << 0)))
    ; // wait until the update flag is set
}

void Delay_micro_second(uint32_t delay) {
  TIM2->CNT = 0;
  while ((uint32_t)TIM2->CNT < delay)
    ;
}

void Delay(uint32_t delay) {
  for (uint16_t i = 0; i < delay; i++) {
    Delay_micro_second(1000);
  }
}