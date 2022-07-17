#include "_debug.h"

uint16_t DEBUG_BUTTON = 0;
uint16_t DEBUG_ANALOG_IO = 0;

void __debugRamUsage() {
  if (!SOTOM_DEBUG_FLAG)
    return;

  uint32_t t_ = getMiliseconds();
  register uint32_t msp __asm("sp");

  if (msp < CURRENT_MSP) {
    CURRENT_MSP = msp;
  }

  if (t_ > _LAST_RAM_USAGE_UPDATE) {
    CURRENT_MSP = STACK_START;
    _LAST_RAM_USAGE_UPDATE = t_ + SRAM_REFRESH_TIME_GAP_MS;
  }
}

void __digitalWriteDebugButton(uint8_t pin, uint8_t value) {
  DEBUG_BUTTON = value? DEBUG_BUTTON|(1 << pin) : DEBUG_BUTTON&~(1<<pin);
}

uint8_t __digitalReadDebugButton(uint8_t pin) {
  return (DEBUG_BUTTON >> pin) & 0b1;
}

uint16_t __alalogReadDebug() { return DEBUG_ANALOG_IO; }
void __analogWriteDebug(uint16_t value) { DEBUG_ANALOG_IO = value; }
