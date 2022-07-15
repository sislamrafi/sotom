#include "LCD162A.h"

void gpioInitForLCD(void) {
  GPIO_ENABLE(GPIOB);
  pinConfig(GPIOB, 0, GPIO_OUTPUT);
  pinConfig(GPIOB, 1, GPIO_OUTPUT);
  pinConfig(GPIOB, 2, GPIO_OUTPUT);
  pinConfig(GPIOB, 3, GPIO_OUTPUT);
  pinConfig(GPIOB, 4, GPIO_OUTPUT);
  pinConfig(GPIOB, 5, GPIO_OUTPUT);
  pinConfig(GPIOB, 6, GPIO_OUTPUT);
  pinConfig(GPIOB, 7, GPIO_OUTPUT);
  pinConfig(GPIOB, 8, GPIO_OUTPUT);
  pinConfig(GPIOB, 9, GPIO_OUTPUT);
  pinConfig(GPIOB, 10, GPIO_OUTPUT);
  Delay_micro_second(LCD_EN_DELAY);
}

void INIT_LCD(void) {
  gpioInitForLCD();
  commToLCD8Bit(0xF);  // on and set cursor
  commToLCD8Bit(0x1);  // clear
  commToLCD8Bit(0x3F); // set line number = 2;
}

void commToLCD8Bit(uint16_t data) {
  data |= (1 << 10);
  GPIOB->BSRR = (0x7FF) << 16; // clear bits
  GPIOB->BSRR = data;

  Delay_micro_second(LCD_EN_DELAY);
  GPIOB->BSRR = (1 << 10) << 16;
  Delay_micro_second(LCD_EN_DELAY);
  GPIOB->BSRR = (0x7FF) << 16; // clear bits
  Delay_micro_second(LCD_EN_DELAY);
}

void printAsciToLCD(char ch) {
  uint16_t data = (uint16_t)ch;
  data |= (1 << 9);
  commToLCD8Bit(data);
}

void printLineToLCD(char *line) {
  for (int i = 0; line[i] != '\0'; i++) {
    if (line[i] == '\n') {
      gotoSecondLineLCD();
      continue;
    }
    printAsciToLCD(line[i]);
  }
}

void clearLCD(void) { commToLCD8Bit(0x1); }

void gotoFirstLineLCD(void) { commToLCD8Bit(0x80); }

void gotoSecondLineLCD(void) { commToLCD8Bit(0xC0); }