#include "timer.h"
// #include "../../protocols/usart/usart.h"
// #include "../stdlib/stdlibc.h"

static uint32_t TIM2_INTERRUPT_CALL_COUNT = 0;
static uint8_t TIM2_READY_TO_USE = 0;

void ConfigTimer2ForSystem(void) {
  // 1. Enable Timer2 clock
  RCC->APB1ENR |= 1 << 0;

  // 2. Set the Prescalar
  TIM2->PSC = TIM2_PSC - 1;

  /*
      THE APB1 BUS CLOCK IS 90MHz
      SO EVERY TIMER DELAY IS 90MHz/90 = 1 microseconds
  */

  // 3. Set the Auto-Reload-Resister value. Set max value for 32 bits.
  TIM2->ARR = (uint32_t)TIM2_ARR;

  // update interrupt enable
  TIM2->DIER = 1 << 0;

  // Re-initialize the counter and generates an update of the registers.
  TIM2->EGR = 1 << 0;

  // 4. Enable the Timer, and wait for the update Flag to set
  TIM2->CR1 |= 1 << 0;
  while (!(TIM2->SR & (1 << 0)))
    ; // wait until the update flag is set

  __NVIC_EnableIRQ(TIM2_IRQn);
  TIM2->SR = ~(1 << 0);

  TIM2->CNT = 0;

  TIM2_READY_TO_USE = 1;
}

void Delay_micro_second(uint32_t delay) {
  uint32_t _ICCTarget = TIM2_INTERRUPT_CALL_COUNT;
  _ICCTarget += (delay / TIM2_ARR);
  uint32_t _CNTTarget = (delay % TIM2_ARR) + (uint32_t)TIM2->CNT;
  _ICCTarget += (_CNTTarget / TIM2_ARR);
  _CNTTarget = (_CNTTarget%TIM2_ARR);
  //usart_println(USART2,itoa(_ICCTarget,10));
  //usart_println(USART2,itoa(_CNTTarget,10));
  while (TIM2_INTERRUPT_CALL_COUNT < _ICCTarget)
    ;
  while ((uint32_t)TIM2->CNT < _CNTTarget)
    ;
}

void Delay(uint32_t delay) {
  for (uint16_t i = 0; i < delay; i++) {
    Delay_micro_second(1000);
  }
}

// TODO: find better way to handel overflow
uint32_t getMiliseconds() {
  uint32_t t_ =
      ((TIM2_ARR * TIM2_INTERRUPT_CALL_COUNT) / TIM2_TICK_TIME_MS_DIV) +
      ((uint32_t)TIM2->CNT / TIM2_TICK_TIME_MS_DIV);
  return t_;
}

// TODO: find better way to handel overflow
uint32_t getMicroseconds() {
  uint32_t t_ =
      ((TIM2_ARR * TIM2_INTERRUPT_CALL_COUNT) / TIM2_TICK_TIME_US_DIV) +
      ((uint32_t)TIM2->CNT / TIM2_TICK_TIME_US_DIV);
  return t_;
}

void TIM2_IRQHandler() {
  TIM2->SR = ~(1 << 0); // clear interrupt flag
  if (!TIM2_READY_TO_USE)
    return;
  TIM2_INTERRUPT_CALL_COUNT++;
  TIM2->CNT = 0;
  SYS_ROUTINE();
}