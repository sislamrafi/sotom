#ifndef __CORE_STARTUP_BOARD_H__
#define __CORE_STARTUP_BOARD_H__

#include <stdint.h> //

#define SRAM_START 0x20000000U
#define SRAM_SIZE (128U * 1024U) // 128KB
#define SRAM_END ((SRAM_START) + (SRAM_SIZE))

#define STACK_START SRAM_END

#endif