#include <msp430.h>
#include <stdint.h>
#include "timer.h"

void wait(uint16_t time, timeUnit_t unit) {
    if (unit == us) {
        TA2CTL = TASSEL__SMCLK  |   // configura passo para 1us
                 MC__UP         |   // com modo up
                 TACLR;             // e zera o contador

        TA2CCR0 = time - 1;

        while(!(TA2CCTL0 & CCIFG)); // espera
        TA2CCTL0 &= ~CCIFG;

        TA2CTL = MC__STOP;          // para contador

        return;
    }
    if (unit == ms) {
        while(time--)
            wait(1000, us);
    }
}
