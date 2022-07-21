/*
    helper lib for debugger
*/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#define SOTOM_DEBUG_FLAG 1
#define SRAM_REFRESH_TIME_GAP_MS 2000

/*
   __debugRamUsage(void):
   this funtion will get the ram usage. This function will take the highest
   value of ram usage of last SRAM_REFRESH_TIME_GAP_MS milliseconds and save the
   value to CURRENT_MSP
*/
void __debugRamUsage(void);
void __digitalWriteDebugButton(uint8_t pin, uint8_t value);
uint8_t __digitalReadDebugButton(uint8_t pin);
uint16_t __alalogReadDebug();
void __analogWriteDebug(uint16_t value);
#endif