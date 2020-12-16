#include <msp430.h>
#include <stdint.h>
#include "gpio.h"

void digitalWrite(uint8_t port, uint8_t bit, uint8_t value) {
    uint8_t mask = 1 << bit;

    uint8_t * POUT = (uint8_t *) (0x200 + 0x002 + (((port-1) >> 1) * 0x20) + (port+1)%2);

    if (value)
        *POUT |= mask;
    else
        *POUT &= ~mask;
}

uint8_t digitalRead (uint8_t port, uint8_t bit) {
    uint8_t mask = 1 << bit;

    // retorna 0 se pino estiver em LOW
    // retorna 1 se pino estiver em HIGH

    uint8_t * PIN = (uint8_t *) (0x200 + (((port-1) >> 1) * 0x20) + (port+1)%2);

    return (*PIN & mask) != 0;
}

void pinMode(uint8_t port, uint8_t bit, enum pinModes mode) {
    uint8_t mask = 1 << bit;

    uint8_t * PDIR = (uint8_t *) (0x200 + 0x004 + (((port-1) >> 1) * 0x20) + (port+1)%2);
    uint8_t * PREN = (uint8_t *) (0x200 + 0x006 + (((port-1) >> 1) * 0x20) + (port+1)%2);
    uint8_t * POUT = (uint8_t *) (0x200 + 0x002 + (((port-1) >> 1) * 0x20) + (port+1)%2);

    if (mode == input) {
        *PDIR &= ~mask;
        *PREN &= ~mask;
        *POUT &= ~mask;
    }
    if (mode == output) {
        *PDIR |= mask;
    }
    if (mode == inPullUp) {
        *PDIR &= ~mask;
        *PREN |= mask;
        *POUT |= mask;
    }
    if (mode == inPullDown) {
        *PDIR &= ~mask;
        *PREN |= mask;
        *POUT &= ~mask;
    }
}
