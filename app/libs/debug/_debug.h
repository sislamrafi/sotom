#ifndef __LIBS_DEBUG_LINKER_H__
#define __LIBS_DEBUG_LINKER_H__

#include "../../main/board.h"
#include "../timer/timer.h"
#include "debug.h"

static uint32_t _LAST_RAM_USAGE_UPDATE = 0;

/* EXPORT VALUES */
volatile uint32_t CURRENT_MSP = STACK_START;

extern uint16_t DEBUG_BUTTON;
extern uint16_t DEBUG_ANALOG_IO;

#endif