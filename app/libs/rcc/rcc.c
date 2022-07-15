#include "rcc.h"

volatile uint32_t SYS_CLOCK_SPEED;

void rcc_config_HSE8MHz_SYS180MHz() {
  const uint16_t PLL_M = 4;
  const uint16_t PLL_N = 180;
  const uint16_t PLL_P = 0;

  // SYS_CLOCK_SPEED = (( (double)(EXTERNAL_CLOCK_SPEED/PLL_M) * (double)PLL_N) / (double)((double)(PLL_P+1)/2.0f));
  SYS_CLOCK_SPEED= (EXTERNAL_CLOCK_SPEED*PLL_N*(PLL_P+1))/(PLL_M*2);

  // 1. ENABLE HSE and wait for the HSE to become Ready
  RCC->CR |= (1 << 16); // 16th bit on for HSE on
  while (!(RCC->CR & (1 << 17)))
    ; // wait for HSE RDY bit to turn on

  // 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
  RCC->APB1ENR |= (1 << 0); // set PWREN bit to 1
  PWR->CR |= (0b11 << 14);  // set scale mode 1

  // 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
  FLASH->ACR |= (1 << 9) | (1 << 10) | (1 < 8) |
                (0b101); // Inctruction cache enable|Data cache enable|prefetch
                         // enable|latency 5ws

  // 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
  RCC->CFGR &= ~(0b1111 << 4); // HPRE; AHB SPEED 180/1; DIV BY 1
  RCC->CFGR = (RCC->CFGR | (0b101 << 10)) &
              ~(0b010 << 10); // PPRE1; APB Low Speed; DIV BY 4
  RCC->CFGR = (RCC->CFGR | (0b100 << 13)) &
              ~(0b011 << 13); // PPRE2; APB High Speed; DIV BY 2

  // 5. Configure the MAIN PLL
  RCC->PLLCFGR = ((PLL_M) | (PLL_N << 6) | (1 << 22)) & ~(PLL_P << 16);

  // 6. Enable the PLL and wait for it to become ready
  RCC->CR |= (1 << 24); // ON PLL
  while (!(RCC->CR & (1 << 25)))
    ; // wait for PLL ready

  // 7. Select the Clock Source and wait for it to be set
  RCC->CFGR = (RCC->CFGR | (0b10 << 0)) & ~(0b1 << 0);
  while (!(RCC->CFGR & (0b10 << 2)))
    ; // wait until PLL is set as system clock
}
