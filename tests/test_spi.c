#include "main.h"
#include "test_runner.h"
#include "hal_spi.h"
#include <stdio.h>
#include <string.h>
#include "fault_injection.h"

#define SPI_FRAME_SIZE 4

#define PROTO_HEADER_REQ  0xA5
#define PROTO_HEADER_RESP 0x5A

#define CS_PORT GPIOB
#define CS_PIN  GPIO_PIN_6


/* ================= LOW LEVEL ================= */

static void spi_select(void)
{
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

static void spi_deselect(void)
{
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

static uint8_t calc_checksum(uint8_t *buf)
{
    return buf[0] ^ buf[1] ^ buf[2];
}

static int response_is_invalid(uint8_t *rx)
{
    if(memcmp(rx, (uint8_t[4]){0,0,0,0}, 4) == 0)
        return 1;

    if(memcmp(rx, (uint8_t[4]){0xFF,0xFF,0xFF,0xFF}, 4) == 0)
        return 1;

    return 0;
}


/* ================= CORE SPI EXECUTION ================= */

static int spi_execute(uint8_t *tx, uint8_t *rx)
{
    spi_select();
    HAL_Delay(2);

    int status = hal_spi_transfer(tx, rx, SPI_FRAME_SIZE);

    spi_deselect();

    return status;
}


/* ================= VALIDATION ================= */

static int validate_response(uint8_t *tx, uint8_t *rx, int status, fault_type_t fault)
{
    if(!status)
    {
        printf("[SPI] FAIL: Transfer failed\r\n");
        return 0;
    }

    if(response_is_invalid(rx))
    {
        printf("[SPI] FAIL: No device / floating bus\r\n");
        return 0;
    }

    int header_ok   = (rx[0] == PROTO_HEADER_RESP);
    int checksum_ok = (rx[3] == calc_checksum(rx));

    /* === Scenario-based evaluation === */

    switch(fault)
    {
        case FI_NONE:
        {
            if(!header_ok)
            {
                printf("[SPI] FAIL: Invalid response header (0x%02X)\r\n", rx[0]);
                return 0;
            }

            if(!checksum_ok)
            {
                printf("[SPI] FAIL: Checksum mismatch\r\n");
                return 0;
            }

            printf("[SPI] PASS: Valid response\r\n");
            return 1;
        }

        case FI_SPI_CORRUPT_RX:
        {
            if(!checksum_ok)
            {
                printf("[SPI] PASS: Corruption detected (checksum mismatch)\r\n");
                return 1;
            }

            printf("[SPI] FAIL: Corruption not detected\r\n");
            return 0;
        }

        case FI_SPI_CORRUPT_TX:
        {
            if(header_ok && checksum_ok)
            {
                printf("[SPI] PASS: Device handled TX corruption\r\n");
                return 1;
            }

            printf("[SPI] FAIL: Invalid response after TX corruption\r\n");
            return 0;
        }

        default:
            printf("[SPI] FAIL: Unknown test case\r\n");
            return 0;
    }
}


/* ================= TEST ================= */

static int run_spi_test(fault_type_t fault)
{
    uint8_t tx[SPI_FRAME_SIZE];
    uint8_t rx[SPI_FRAME_SIZE];

    memset(rx, 0, sizeof(rx));

    tx[0] = PROTO_HEADER_REQ;
    tx[1] = 0x01;
    tx[2] = 0x55;
    tx[3] = calc_checksum(tx);

    fault_set(fault);

    int status = spi_execute(tx, rx);

    printf("TX: %02X %02X %02X %02X\r\n", tx[0], tx[1], tx[2], tx[3]);
    printf("RX: %02X %02X %02X %02X\r\n", rx[0], rx[1], rx[2], rx[3]);

    int pass = validate_response(tx, rx, status, fault);

    printf("[TEST][SPI] RESULT: %s\r\n\r\n", pass ? "PASS" : "FAIL");
    report_result(pass);

    return pass;
}


/* ================= TEST MATRIX ================= */

void run_spi_matrix(void)
{
    printf("\r\n=== SPI TEST MATRIX ===\r\n\r\n");

    printf("SPI_NORMAL\r\n");
    run_spi_test(FI_NONE);

    printf("\r\nSPI_CORRUPT_RX\r\n");
    run_spi_test(FI_SPI_CORRUPT_RX);

    printf("\r\nSPI_CORRUPT_TX\r\n");
    run_spi_test(FI_SPI_CORRUPT_TX);

    fault_set(FI_NONE);
}
