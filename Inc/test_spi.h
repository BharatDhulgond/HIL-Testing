#ifndef TEST_SPI_H
#define TEST_SPI_H

typedef enum
{
    SPI_TEST_NORMAL,
    SPI_TEST_CORRUPT_RX,
    SPI_TEST_CORRUPT_TX,
    SPI_TEST_BAD_HEADER,
    SPI_TEST_TIMEOUT
} spi_test_case_t;

void run_spi_matrix(void);

#endif
