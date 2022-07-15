#include "startup_stm32f446xx.h"

void Default_Handler(void) {
  while (1)
    ;
}

void Default_Handler_Loop(void) {
  // do nothing
}

#define RAM_USAGE_SAMPLE_COUNT_LEN 10000000
static uint32_t RAM_USAGE_SAMPLE_COUNT = 0;
void __debugRamUsage() {
  RAM_USAGE_SAMPLE_COUNT++;

  register uint32_t msp __asm("sp");

  if (msp < CURRENT_MSP){
    CURRENT_MSP = msp;
    RAM_USAGE_SAMPLE_COUNT = 0;
  }

  if (RAM_USAGE_SAMPLE_COUNT > RAM_USAGE_SAMPLE_COUNT_LEN){
    RAM_USAGE_SAMPLE_COUNT = 0;
    CURRENT_MSP = STACK_START;
  }
}

/*
    1)  copy .data section to sram
    2)  init the .bss section to 0x0 sram
    3)  call init function of init library (if used)
    4)  call main()
*/
void Reset_Handler(void) {
  uint32_t size = 0;

  size = (uint32_t)&_edata - (uint32_t)&_sdata;
  uint8_t *pDst = (uint8_t *)&_sdata;
  uint8_t *pSrc = (uint8_t *)&_etext;
  for (uint32_t i = 0; i < size; i++) {
    *pDst++ = *pSrc++;
  }

  size = (uint32_t)&_ebss - (uint32_t)&_sbss;
  pDst = (uint8_t *)&_sbss;
  for (uint32_t i = 0; i < size; i++) {
    *pDst++ = 0;
  }

  _text_size = (uint32_t)&_etext - (uint32_t)&_stext;
  _data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
  _bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;

  main();
}