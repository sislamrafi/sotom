#ifndef __SPI_H__
#define __SPI_H__

#include "../registers/external_peripherals.h"
#include "../registers/internal_peripherals.h"

// TYPE DEFINITION
typedef enum SPI_CONFIG_e
{
    /*
     ***CONFIG CR1 REGISTER***
     */
    SPI_CR1_MASK = 0XFFFF,

    // BIDIMODE: Bidirectional mode
    SPI_BIDIMODE_ENABLE = 0b1000000000000000,
    SPI_BIDIMODE_Pos = 15,

    // BIDIOE: Output enable in bidirectional mode
    SPI_BIDI_OUTPUT_ENABLE = 0b100000000000000,
    // SPI_BIDI_OUTPUT_DISABLE = 0b0,
    SPI_BIDIOE_Pos = 14,

    // CRCEN: CRC calculation enable
    SPI_CRC_ENABLE = 0b10000000000000,
    SPI_CRC_Pos = 13,

    // DFF: Data frame format
    SPI_16BIT = 0b100000000000,
    SPI_8BIT = 0b0,
    SPI_DFF_Pos = 11,

    // RXONLY : Receive only mode enable
    SPI_RXONLY = 0b10000000000,
    SPI_FULL_DUPLEX = 0b0,
    SPI_RXONLY_Pos = 10,

    // SSM : Software slave management
    SPI_SSM_ENABLE = 0b1000000000,
    SSM_Pos = 9,

    // SSI : Internal slave select
    SPI_SSI_ENABLE = 0b100000000,
    SSI_Pos = 8,

    // LSBFIRST : Frame format
    SPI_LSBFIRST = 0b10000000,
    SPI_MSBFIRST = 0b0,
    SPI_LSBFIRST_Pos = 7,

    // BR[2:0]: Baud rate control
    SPI_BR_2 = 0b000000,
    SPI_BR_4 = 0b001000,
    SPI_BR_8 = 0b010000,
    SPI_BR_16 = 0b011000,
    SPI_BR_32 = 0b100000,
    SPI_BR_64 = 0b101000,
    SPI_BR_128 = 0b110000,
    SPI_BR_256 = 0b111000,
    SPI_BR_Pos = 3,
    SPI_BR_Mask = 0b111,

    // MSTR : Master selection
    SPI_MASTER = 0b100,
    SPI_SLAVE = 0b0,
    SPI_MSTR_Pos = 2,

    // CPOL : Clock polarity
    SPI_CPOL_HIGH = 0b10,
    SPI_CPOL_LOW = 0b0,
    SPI_CPOL_Pos = 1,

    // CPHA : Clock phase
    SPI_CPHA_FIRST = 0b1,
    SPI_CPHA_SECOND = 0b0,
    SPI_CPHA_Pos = 0,

    /*
     ***CONFIG CR2 REGISTER***
     */
    SPI_CR2_MASK = 0XF70000,

    // TXEIE: Tx buffer empty interrupt enable
    SPI_TXEIE_ENABLE = 0b100000000000000000000000,
    SPI_TXEIE_Pos = 23,

    // RXNEIE: RX buffer not empty interrupt enable
    SPI_RXNEIE_ENABLE = 0b10000000000000000000000,
    SPI_RXNEIE_Pos = 22,

    // ERRIE: Error interrupt enable
    SPI_ERRIE_ENABLE = 0b1000000000000000000000,
    SPI_ERRIE_Pos = 21,

    // FRF: Frame format
    SPI_FRF_TI = 0b100000000000000000000,
    SPI_FRF_MOTOROLA = 0b0,
    SPI_FRF_Pos = 20,

    // SSOE: SS output enable
    SPI_SSOE_ENABLE = 0b1000000000000000000,
    SPI_SSOE_Pos = 18,

    // TXDMAEN: Tx buffer DMA enable
    SPI_TXDMA_ENABLE = 0b100000000000000000,
    SPI_TXDMA_Pos = 17,

    // RXDMAEN: RX buffer DMA enable
    SPI_RXDMA_ENABLE = 0b10000000000000000,
    SPI_RXDMA_Pos = 16,

} SPI_CONFIG;

// FUNCTION PROTOTYPE

// SPI_ENABLE(SPI_T, MASTER/SLAVE, CLOCK_POLARITY, CLOCK_PHASE, CLOCK_DIVIDER, DUPLEXITY, LSB_FIRST/MSB_FIRST, HALF_DUPLEX/FULL_DUPLEX)
void SPI_CONFIG(SPI_t *spi, SPI_CONFIG config);
void SPI_ENABLE(SPI_t *spi);
void SPI_DISABLE(SPI_t *spi);
void SPI_TRANSMIT();
void SPI_RECIEVE();
#endif

/*
    PIN CONIG:
    SPI1:
        SCK: PA5, PB3
        MISO: PA6, PB4
        MOSI: PA7, PB5
        SSEL: PA4, PA15
    SPI2:
        SCK: PB13, PB10
        MISO: PB14, PC2
        MOSI: PB15. PC3
        SSEL: PB9, PB12
    SPI3:
        SCK: PC10
        MISO: PC11
        MOSI: PC12
*/
