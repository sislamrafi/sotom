#ifndef __GPIO_H__
#define __GPIO_H__

#include "../registers/external_peripherals.h"
#include "../registers/internal_peripherals.h"

typedef enum GPIO_CONFIG_e {
  // MODER_REG 0b 00 00 00 xx
  GPIO_INPUT = 0b00,
  GPIO_OUTPUT = 0b01,
  GPIO_ALTERNET = 0b10,
  GPIO_ANALOG = 0b11,

  GPIO_MODER_MASK = 0b11,
  GPIO_MODER_MASK_Pos = 0,

  // PUPD_REG  0b 00 00 xx 00
  GPIO_PULL_UP = 0b0100,
  GPIO_PULL_DOWN = 0b1000,

  GPIO_PUPD_MASK = 0b1100,
  GPIO_PUPD_MASK_Pos = 2,

  // OTYPE_REG 0b 00 Xx 00 00
  GPIO_PUSH_PULL = 0b00000000,
  GPIO_OPEN_DRAIN = 0b00010000,

  GPIO_OTYPE_MASK = 0b110000,
  GPIO_OTYPE_MASK_Pos = 4,

  // OSPEEDR_REG 0b xx 00 00 00
  GPIO_LOW_SPEED = 0b00000000,
  GPIO_MIDIUM_SPEED = 0b01000000,
  GPIO_FAST_SPEED = 0b10000000,
  GPIO_HIGH_SPEED = 0b11000000,

  GPIO_OSPEEDR_MASK = 0b11000000,
  GPIO_OSPEEDR_MASK_Pos = 6,

  // AFRL_REG
  GPIO_AF_AF0 = 0x000,
  GPIO_AF_AF1 = 0x100,
  GPIO_AF_AF2 = 0x200,
  GPIO_AF_AF3 = 0x300,
  GPIO_AF_AF4 = 0x400,
  GPIO_AF_AF5 = 0x500,
  GPIO_AF_AF6 = 0x600,
  GPIO_AF_AF7 = 0x700,
  GPIO_AF_AF8 = 0x800,
  GPIO_AF_AF9 = 0x900,
  GPIO_AF_AF10 = 0xA00,
  GPIO_AF_AF11 = 0xB00,
  GPIO_AF_AF12 = 0xC00,
  GPIO_AF_AF13 = 0xD00,
  GPIO_AF_AF14 = 0xE00,
  GPIO_AF_AF15 = 0xF00,

  GPIO_AF_MASK = 0b111100000000,
  GPIO_AF_MASK_Pos = 6,
} GPIO_CONFIG;

typedef enum GPIO_PIN_VALUE_e {
  GPIO_PIN_HIGH = 1,
  GPIO_PIN_LOW = 0,
} GPIO_PIN_VALUE;

typedef enum GPIO_INTERRUPT_TRIGGER_e {
  GPIO_INTERRUPT_TRIGGER_RISING = 0b01,
  GPIO_INTERRUPT_TRIGGER_FALLING = 0b10,
} GPIO_INTERRUPT_TRIGGER;

void GPIO_ENABLE(GPIO_t *port);
void pinConfig(GPIO_t *port, uint8_t pin, GPIO_CONFIG config);
void digitalWrite(GPIO_t *port, uint8_t pin, GPIO_PIN_VALUE value);
GPIO_PIN_VALUE digitalRead(GPIO_t *port, uint8_t pin);

void pinInterruptConfig(GPIO_t *port, uint8_t pin,
                        GPIO_INTERRUPT_TRIGGER trigger, uint8_t priority);
uint8_t isInterruptPending(GPIO_t *port, uint8_t pin);

#endif