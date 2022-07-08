#include "spi.h"

void SPI_CONFIG(SPI_t *spi, SPI_CONFIG config)
{
    switch ((uint32_t)spi)
    {
    case (uint32_t)SPI1:
        RCC->APB2ENR |= (1 << 12);
        break;
    case (uint32_t)SPI2:
        RCC->APB1ENR |= (1 << 14);
        break;
    case (uint32_t)SPI3:
        RCC->APB1ENR |= (1 << 15);
        break;
    case (uint32_t)SPI4:
        RCC->APB2ENR |= (1 << 12);
        break;
    default:
        break;
    }
    spi->CR1 = (((uint32_t)config >> SPI_CR1_Pos) & SPI_CR1_MASK);
    spi->CR2 = (((uint32_t)config >> SPI_CR2_Pos) & SPI_CR2_MASK);
}

void SPI_ENABLE(SPI_t *spi)
{
    spi->CR1 |= (1 << 6);
}
void SPI_DISABLE(SPI_t *spi)
{
    // Wait until RXNE=1 to receive the last data.
    while ((spi->SR & (1 << 0)))
        ;
    // Wait until TXE=1 and then wait until BSY=0 before disabling the SPI.
    while (!((SPI1->SR) & (1 << 1)))
        ;
    while (((SPI1->SR) & (1 << 7)))
        ;
    // Read received data.
    uint8_t temp = spi->DR;
    // Disable SPI
    spi->CR1 &= ~(1 << 6);

    /*
        The correct disable procedure for certain receive-only modes is:
            1. Interrupt the receive flow by disabling SPI (SPE=0) in the specific time window while
            the last data frame is ongoing.
            2. Wait until BSY=0 (the last data frame is processed).
            3. Read received data.

    */
}
void SPI_TRANSMIT(SPI_t *spi, uint8_t data, uint32_t datasize)
{
    for (uint32_t i = 0; i < size; i++)
    {
        // Wait until TXE=1. Indicates that TX-buffer is empty and ready to transmit.
        while (!((spi->SR) & (1 << 1)))
            ;
        // Write data to be transmitted.
        spi->DR = data;
        // Wait until BSY=0.
        while ((spi->SR) & (1 << 7))
            ;
    }
    // Wait until TXE=1 and BSY=0.
    while ((!((spi->SR) & (1 << 1))) | ((spi->SR) & (1 << 7)))
        ;
    // Read DR and SR to clear overrun.
    uint8_t temp = (spi->DR | spi->SR);
}
void SPI_RECIEVE(SPI_t *spi, uint8_t *data, uint32_t datasize)
{
    for (uint32_t i = 0; i < size; i++)
    {
        // Wait until BSY=0.
        while ((spi->SR) & (1 << 7))
            ;
        // Send dummy data to keep CLOCK active.
        spi->DR = 0x00;
        // Wait until RXNE=1. Indicates that RX-buffer is not empty and ready to receive.
        while (!((spi->SR) & (1 << 0)))
            ;
        // Read received data.
        data[i] = spi->DR;
    }
}
