#include "main.h"

#define LED_PIN 0x04
#define PUSH_BUTTON1 0x01U
#define ON_BORD_LED 0x05U

volatile int a = 10;
uint8_t b[22];
uint16_t penMan = 160;
uint8_t penMan1 = 160;
uint32_t penMan11 = 161;
uint8_t penMan2 = 16;
uint8_t penMan3 = 16;
uint8_t penMan4 = 16;
uint8_t penMan5 = 16;

int main(void) {
  int bb[100];
  for (uint16_t i = 0; i < 100; i++) {
    bb[i] = 56;
  }

  rcc_config_HSE8MHz_SYS180MHz();
  ConfigTimer2ForSystem();
  configUSART(USART2, USART2_PIN_A3_A2, 9600L);

  INIT_LCD();

  GPIO_ENABLE(GPIOA);
  pinConfig(GPIOA, LED_PIN, GPIO_OUTPUT);
  pinConfig(GPIOA, ON_BORD_LED, GPIO_OUTPUT);

  pinConfig(GPIOA, PUSH_BUTTON1, GPIO_INPUT | GPIO_PULL_DOWN | GPIO_LOCK_PIN);

  usart_println(USART2, "Hello From Stm32");

  if (isPinLocked(GPIOA, PUSH_BUTTON1)) {
    digitalWrite(GPIOA, ON_BORD_LED, GPIO_PIN_HIGH);
  } else {
    digitalWrite(GPIOA, ON_BORD_LED, GPIO_PIN_LOW);
  }

  pinInterruptConfig(
      GPIOA, PUSH_BUTTON1,
      GPIO_INTERRUPT_TRIGGER_FALLING | GPIO_INTERRUPT_TRIGGER_RISING, 1);

  // clearLCD();
  // Delay(3000);
  // printLineToLCD("INIT COMPLETE!!\nHELLO WORLD. :)");

  //__debugRamUsage();
  uint32_t aa = 0;
  while (1) {
    /* code */
    clearLCD();
    printLineToLCD("Analog Button: \n");
    printLineToLCD(itoa(__alalogReadDebug(),10));
    digitalWrite(GPIOA, ON_BORD_LED, GPIO_PIN_HIGH);
    __digitalWriteDebugButton(ON_BORD_LED,GPIO_PIN_HIGH);
    __digitalWriteDebugButton(12,GPIO_PIN_HIGH);
    Delay(2000);
    digitalWrite(GPIOA, ON_BORD_LED, GPIO_PIN_LOW);
    __digitalWriteDebugButton(ON_BORD_LED,GPIO_PIN_LOW);
    __digitalWriteDebugButton(12,GPIO_PIN_LOW);
    clearLCD();
    printLineToLCD("Analog Button: \n");
    printLineToLCD(itoa(__alalogReadDebug(),10));
    Delay(2000);
  }
}

void SYS_ROUTINE(void) {
  __debugRamUsage();
  //usart_println(USART2, itoa(getMicroseconds(),10));
}

void EXTI1_IRQHandler(void) {
  int a[10000];
  for (uint16_t i = 0; i < 10000; i++) {
    a[i] = 56;
  }
  //__debugRamUsage();
  if (isInterruptPending(GPIOA, PUSH_BUTTON1)) {
    Delay(20);
    __digitalWriteDebugButton(0,digitalRead(GPIOA, PUSH_BUTTON1));
    digitalWrite(GPIOA, LED_PIN, digitalRead(GPIOA, PUSH_BUTTON1));
    clearLCD();
    printLineToLCD("BUTTON1 PRESSED\n");
    printLineToLCD("TIM2 : ");
    printLineToLCD(itoa(getMiliseconds(), 10));
    if (digitalRead(GPIOA, PUSH_BUTTON1))
      printLineToLCD("LED ONN");
    else
      printLineToLCD("LED OFF");
    return;
  }
}

void USART2_IRQHandler(void) {
  // if RXNE bit is high
  if (USART2->SR & (1 << 5)) {
    digitalWrite(GPIOA, LED_PIN, 0);
  }
}