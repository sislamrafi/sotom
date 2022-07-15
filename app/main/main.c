#include "main.h"


#define LED_PIN 0x04
#define PUSH_BUTTON1 0x01U
#define ON_BORD_LED 0x05U

volatile int a = 10;
int b[21];

int main(void) {
  int bb[100];
  for (uint16_t i = 0; i < 100; i++) {
    bb[i] = 56;
  }

  rcc_config_HSE8MHz_SYS180MHz();
  ConfigTimer2ForDelay();

  configUSART(USART2, USART2_PIN_A3_A2, 9600L);

  INIT_LCD();

  GPIO_ENABLE(GPIOA);
  pinConfig(GPIOA, LED_PIN, GPIO_OUTPUT);
  pinConfig(GPIOA, ON_BORD_LED, GPIO_OUTPUT);

  pinConfig(GPIOA, PUSH_BUTTON1, GPIO_INPUT | GPIO_PULL_DOWN | GPIO_LOCK_PIN);
  pinConfig(GPIOA, PUSH_BUTTON1, GPIO_OUTPUT);

  // pinConfig(GPIOA, PUSH_BUTTON1, GPIO_OUTPUT);
  usart_println(USART2, "Hello from usart 2");

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

  __debugRamUsage();

  while (1) {
    /* code */
    __debugRamUsage();
  }
}

uint8_t abc = 0;
void SYS_ROUTINE(void) {
  return ;
  uint32_t ss = getMicroseconds();
  digitalWrite(GPIOA, LED_PIN, abc);
  abc = abc ? 0 : 1;
  __debugRamUsage();
  uint32_t sm = getMicroseconds();
  clearLCD();
  printLineToLCD("MILI : ");
  printLineToLCD(itoa(sm - ss, 10));
  printLineToLCD("\nMICRO: ");
  printLineToLCD(itoa(sm, 10));
  // Delay(5000);
}

void EXTI1_IRQHandler(void) {
  int a[10000];
  for (uint16_t i = 0; i < 10000; i++) {
    a[i] = 56;
  }
  __debugRamUsage();
  if (isInterruptPending(GPIOA, PUSH_BUTTON1)) {
    Delay(20);
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
    digitalWrite(GPIOA, LED_PIN, 1);
  }
}