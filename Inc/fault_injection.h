#ifndef FAULT_INJECTION_H
#define FAULT_INJECTION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Fault types supported by the HIL framework
 * These faults simulate communication errors
 * to validate robustness of embedded drivers.
 */
typedef enum
{
    FI_NONE = 0,          /* No fault */

    FI_SPI_CORRUPT_TX,    /* Corrupt outgoing SPI frame */
    FI_SPI_CORRUPT_RX,    /* Corrupt received SPI frame */

    FI_SPI_BAD_HEADER,    /* Destroy protocol header */

    FI_TIMEOUT            /* Simulate SPI timeout */

} fault_type_t;


/* Set active fault type */
void fault_set(fault_type_t type);

/* Get currently active fault */
fault_type_t fault_get(void);

/* Inject faults into TX frame */
void fault_inject_tx(uint8_t *buf, uint16_t len);

/* Inject faults into RX frame */
void fault_inject_rx(uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* FAULT_INJECTION_H */
