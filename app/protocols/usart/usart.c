#include "usart.h"

void configUSART(USART_t *usart, USART_CONFIG config, uint32_t baudrate) {
  // enable gpios
  GPIO_ENABLE(config.RX.GPIO);
  GPIO_ENABLE(config.TX.GPIO);

  // config pins
  pinConfig(config.RX.GPIO, config.RX.pin,
            GPIO_ALTERNET | config.RX.af | GPIO_HIGH_SPEED | GPIO_LOCK_PIN);
  pinConfig(config.TX.GPIO, config.TX.pin,
            GPIO_ALTERNET | config.TX.af | GPIO_HIGH_SPEED | GPIO_LOCK_PIN);

  // enable the usart
  *config.EN.REG |= (1 << (config.EN.bit));

  usart->CR1 |= (1 << 13);           // enable ue bit
  usart->BRR = 45000000L / baudrate; // set baudrate
  usart->CR1 |= (1 << 3);            // enable te bit
  usart->CR1 |= (1 << 2);            // enable re bit

  usart->CR1 |= (1 << 5);        // set rxe interrupt
  __NVIC_EnableIRQ(config.IRQn); // enable interrupt
}

void usartTx(USART_t *usart, char ch) {
  while (!(usart->SR & (1 << 7)))
    ;
  usart->DR = (ch & 0xFF);
}

char usartRx(USART_t *usart) {
  while (!(usart->SR & (1 << 5)))
    ;
  return (usart->DR & 0xFF);
}

void usart_getln(USART_t *usart, char *line, int len) {
  int counter = 0;
  while (counter < len) {
    line[counter] = usartRx(usart);
    counter++;
    if (counter >= 2 && line[counter - 1] == '\n' &&
        line[counter - 2] == '\r') {
      line[counter - 2] = '\0';
      break;
    }
  }
  line[counter] = '\0';
}

void usart_println(USART_t *usart, char str[]) {
  for (int i = 0; str[i] != '\0'; i++) {
    usartTx(usart, str[i]);
  }
  usartTx(usart, '\r');
  usartTx(usart, '\n');
}