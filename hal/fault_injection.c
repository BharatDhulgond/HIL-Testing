#include "fault_injection.h"
#include <stddef.h>

static fault_type_t current_fault = FI_NONE;

void fault_set(fault_type_t type)
{
    current_fault = type;
}

fault_type_t fault_get(void)
{
    return current_fault;
}


/* Inject corruption into TX frame */
void fault_inject_tx(uint8_t *buf, uint16_t len)
{
    if(buf == NULL || len < 2)
        return;

    if (current_fault == FI_SPI_CORRUPT_TX)
    {
        buf[1] ^= 0x01;   // flip command bit
    }

    if (current_fault == FI_SPI_BAD_HEADER)
    {
        buf[0] = 0x00;    // destroy protocol header
    }
}


/* Inject corruption into RX frame */
void fault_inject_rx(uint8_t *buf, uint16_t len)
{
    if(buf == NULL || len < 3)
        return;

    if (current_fault == FI_SPI_CORRUPT_RX)
    {
        buf[2] ^= 0x01;   // flip payload bit
    }
}
