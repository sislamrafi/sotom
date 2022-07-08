#include "../LCD162A/LCD162A.h"
#include "../gpio/gpio.h"
#include "../rcc/rcc.h"
#include "../registers/external_peripherals.h"
#include "../registers/internal_peripherals.h"
#include "../stdlib/stdlibc.h"
#include "../timer/timer.h"
#include <stdint.h>

#define LED_PIN 0x04
#define PUSH_BUTTON1 0x01U
#define PUSH_BUTTON2 0x00U
#define ON_BORD_LED 0x05U

int main(void) {
  rcc_config_HSE8MHz_SYS180MHz();
  ConfigTimer2ForDelay();

  INIT_LCD();

  GPIO_ENABLE(GPIOA);
  pinConfig(GPIOA, PUSH_BUTTON2, GPIO_INPUT | GPIO_PULL_DOWN );
  GPIOA->LCKR|=((1<<16)|(1<<PUSH_BUTTON2));
  pinConfig(GPIOA, PUSH_BUTTON1, GPIO_INPUT | GPIO_PULL_DOWN);
  pinConfig(GPIOA, LED_PIN, GPIO_OUTPUT);
  pinConfig(GPIOA, ON_BORD_LED, GPIO_OUTPUT);

  pinInterruptConfig(
      GPIOA, PUSH_BUTTON1,
      GPIO_INTERRUPT_TRIGGER_FALLING | GPIO_INTERRUPT_TRIGGER_RISING, 1);

  pinInterruptConfig(GPIOA, PUSH_BUTTON2, GPIO_INTERRUPT_TRIGGER_FALLING, 2);

  clearLCD();
  printLineToLCD("INIT COMPLETE!\nHELLO WORLD. :)");

  while (1) {
    digitalWrite(GPIOA, ON_BORD_LED, GPIO_PIN_HIGH);
    Delay(3000);
    digitalWrite(GPIOA, ON_BORD_LED, GPIO_PIN_LOW);
    Delay(3000);
  }
}

void EXTI1_IRQHandler(void) {
  if (isInterruptPending(GPIOA, PUSH_BUTTON1)) {
    Delay(20);
    digitalWrite(GPIOA, LED_PIN, digitalRead(GPIOA, PUSH_BUTTON1));
    clearLCD();
    printLineToLCD("BUTTON1 PRESSED\n");
    if (digitalRead(GPIOA, PUSH_BUTTON1))
      printLineToLCD("LED ON");
    else
      printLineToLCD("LED OFF");
    return;
  }
}

static uint8_t LIGHT_STATE = 0;
void EXTI0_IRQHandler(void) {
  if (isInterruptPending(GPIOA, PUSH_BUTTON2)) {
    LIGHT_STATE++;
    clearLCD();
    printLineToLCD("BUTTON2 PRESSED\nEXTIO_COUNT= ");
    printLineToLCD(itoa(LIGHT_STATE, 10));
    for (int i = 0; i < LIGHT_STATE; i++) {
      digitalWrite(GPIOA, LED_PIN, GPIO_PIN_HIGH);
      Delay(50);
      digitalWrite(GPIOA, LED_PIN, GPIO_PIN_LOW);
      Delay(50);
    }
  }
}
