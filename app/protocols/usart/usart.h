#ifndef __USART_H__
#define __USART_H__

#define __USART_H__

#include "../../main/board.h"
#include "../../libs/gpio/gpio.h"

typedef struct __BitLocation_t {
  uint32_t *REG;
  uint8_t bit;
} BitLocation;

typedef struct USART_CONFIG_t {
  BitLocation EN;
  PinWithFunction RX;
  PinWithFunction TX;
  PinWithFunction RTS;
  PinWithFunction CTS;
  uint8_t IRQn;
} USART_CONFIG;

// firstrx, then tx
static const USART_CONFIG USART2_PIN_A3_A2
    __attribute__((unused)) = {.EN = {(uint32_t *)(&(RCC->APB1ENR)), 17},
                               .RX = {GPIOA, 3, GPIO_AF_AF7},
                               .TX = {GPIOA, 2, GPIO_AF_AF7},
                               .IRQn = USART2_IRQn};

void configUSART(USART_t *usart, USART_CONFIG config, uint32_t baudrate);
void usartTx(USART_t *usart, char ch);
char usartRx(USART_t *usart);
void usart_getln(USART_t *usart, char *line, int len);
void usart_println(USART_t *usart, char str[]);

#endif