#include <msp430.h>
#include <stdint.h>
#include "i2c.h"
#include "gpio.h"
#include "timer.h"

/* --------------------- Camada física -------------------------- */

void i2cReleaseSDA() {
    // Botar o pino SDA em estado de alta impedancia
    // Pino em Dreno aberto
    P4DIR &= ~BIT1;
    P4REN |= BIT1;
    P4OUT |= BIT1;
}

void i2cReleaseSCL() {
    // Botar o pino SCL em estado de alta impedancia
    // Pino em Dreno aberto
    P4DIR &= ~BIT2;
    P4REN |= BIT2;
    P4OUT |= BIT2;
}

void i2cPullLowSDA() {
    P4OUT &= ~BIT1;
    P4DIR |= BIT1;
}

void i2cPullLowSCL() {
    P4OUT &= ~BIT2;
    P4DIR |= BIT2;
}

/* --------------------- Ações de 1 bit ------------------------- */

void i2cSendBit(uint8_t bitValue) {
    if (bitValue)
        i2cReleaseSDA();
    else
        i2cPullLowSDA();

    wait(TCLK4);

    i2cReleaseSCL();

    wait(TCLK4);
    wait(TCLK4);

    i2cPullLowSCL();

    wait(TCLK4);
}

/* --------------------- Ações multibit ------------------------- */

void i2cSendByteBB(uint8_t byte) {
    uint8_t mask = BIT7;            // Mascara para filtrar os bits

    while(mask) {
        i2cSendBit(byte & mask);    // Envia do mais significativo
        mask >>= 1;                 // Desloca bit da mascara
    }
}

