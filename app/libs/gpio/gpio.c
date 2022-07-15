#include "gpio.h"

void GPIO_ENABLE(GPIO_t *port) {
  switch ((uint32_t)port) {
  case (uint32_t)GPIOA:
    RCC->AHB1ENR |= 1;
    break;
  case (uint32_t)GPIOB:
    RCC->AHB1ENR |= 0b10;
    break;
  case (uint32_t)GPIOC:
    RCC->AHB1ENR |= 0b100;
    break;
  case (uint32_t)GPIOD:
    RCC->AHB1ENR |= 0b1000;
    break;
  case (uint32_t)GPIOE:
    RCC->AHB1ENR |= 0b10000;
    break;
  case (uint32_t)GPIOF:
    RCC->AHB1ENR |= 0b100000;
    break;
  default:
    break;
  }
}

void pinConfig(GPIO_t *port, uint8_t pin, uint32_t config) {
  // set moder to 00
  // then set mode
  port->MODER &= ~(0b11 << (pin * 2));
  port->MODER |=
      (((config & GPIO_MODER_MASK) >> GPIO_MODER_MASK_Pos) << (pin * 2));

  // set pupd  to 00
  // then set pupd
  port->PUPDR &= ~(0b11 << (pin * 2));
  port->PUPDR |= (((config >> GPIO_PUPD_MASK_Pos) & 0b11) << (pin * 2));

  // set otyper to 0
  // then set otyper
  port->OTYPER &= ~(0b1 << (pin));
  port->OTYPER |= (((config >> GPIO_OTYPE_MASK_Pos) & 0b1) << (pin));

  // set OSPEEDR to 00
  // then set OSPEEDR
  port->OSPEEDR &= ~(0b11 << (pin * 2));
  port->OSPEEDR |= (((config >> GPIO_OSPEEDR_MASK_Pos) & 0b11) << (pin * 2));

  if (pin <= 7) {
    // set AFRL to 0b0000
    // then set AFRL
    port->AFRL &= ~(0b1111 << (pin * 4));
    port->AFRL |= (((config >> GPIO_AF_MASK_Pos) & 0b1111) << (pin * 4));
  } else {
    // set AFRH to 0b0000
    // then set AFRH
    port->AFRH &= ~(0b1111 << ((pin - 8) * 4));
    port->AFRH |= (((config >> GPIO_AF_MASK_Pos) & 0b1111) << ((pin - 8) * 4));
  }

  if (((config >> GPIO_LOCK_PIN_Pos) & 0b1)) {
    uint32_t lcr = (1 << pin);
    lcr |= (1 << 16);
    port->LCKR = lcr;
    lcr &= ~(1 << 16);
    port->LCKR = lcr;
    lcr |= (1 << 16);
    port->LCKR = lcr;

    lcr = port->LCKR;
  }
}

void digitalWrite(GPIO_t *port, uint8_t pin, uint8_t value) {
  port->BSRR = value ? 1 << pin : (1 << pin) << 16;
}

GPIO_PIN_VALUE digitalRead(GPIO_t *port, uint8_t pin) {
  return ((port->IDR >> pin) & 0b1);
}

uint8_t isPinLocked(GPIO_t *port, uint8_t pin) {
  uint32_t lcr = port->LCKR;
  return (lcr >> pin) & 0b1;
}

void pinInterruptConfig(GPIO_t *port, uint8_t pin,
                        uint8_t trigger, uint8_t priority) {
  RCC->APB2ENR |= (1 << 14); // Enable SYSCNFG Clock

  uint8_t EXTICode = port == GPIOA   ? 0x0
                     : port == GPIOB ? 0x1
                     : port == GPIOC ? 0x2
                     : port == GPIOD ? 0x3
                     : port == GPIOE ? 0x4
                     : port == GPIOF ? 0x5
                     : port == GPIOG ? 0x6
                                     : 0x7;

  // set EXTICR
  SYSCFG->EXTICR[(uint8_t)(pin / 4)] &= ~(0xf << (4 * (pin % 4)));
  SYSCFG->EXTICR[(uint8_t)(pin / 4)] |= (EXTICode << (4 * (pin % 4)));

  EXTI->IMR |= (1 << pin);                       // Disable mask on EXTI1
  EXTI->RTSR |= ((trigger & 0b1) << pin);        // set rising triger
  EXTI->FTSR |= (((trigger >> 1) & 0b1) << pin); // set falling triger

  IRQn_Type _IRQn;

  _IRQn = pin == 0   ? EXTI0_IRQn
          : pin == 1 ? EXTI1_IRQn
          : pin == 2 ? EXTI2_IRQn
          : pin == 3 ? EXTI3_IRQn
          : pin == 4 ? EXTI4_IRQn
          : pin <= 9 ? EXTI9_5_IRQn
                     : EXTI15_10_IRQn;

  if (priority > 0)
    __NVIC_SetPriority(_IRQn, priority);
  __NVIC_EnableIRQ(_IRQn);
}

uint8_t isInterruptPending(GPIO_t *port, uint8_t pin) {
  if (EXTI->PR & (1 << pin)) {
    EXTI->PR |= (1 << pin);
    return 1;
  }
  return 0;
}
