#ifndef __LCD162A_H_
#define __LCD162A_H_

#include "../../../libs/timer/timer.h"
#include "../../../libs/gpio/gpio.h"

#define LCD_EN_DELAY 1200//in microsecond

void INIT_LCD(void);
void gpioInitForLCD(void);
void commToLCD8Bit(uint16_t data);
void printAsciToLCD(char ch);
void printLineToLCD(char *line);
void clearLCD(void);
void gotoSecondLineLCD(void);
void gotoFirstLineLCD(void);

#endif