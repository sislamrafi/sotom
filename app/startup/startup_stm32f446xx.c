#include "startup_stm32f446xx.h"

void Default_Handler(void) {
  while (1)
    ;
}

/*
    1)  copy .data section to sram
    2)  init the .bss section to 0x0 sram
    3)  call init function of init library (if used)
    4)  call main()
*/
uint32_t size = 0;
void Reset_Handler(void) {
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

  main();
}