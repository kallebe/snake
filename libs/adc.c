#include <msp430.h>
#include "adc.h"

void adcInit() {
    ADC12CTL0 = ~ADC12ENC;          // Desabilita conversoes

    P6SEL |= (BIT0 | BIT1);         // Habilita P6.0 e P6.1

    ADC12CTL0 = ADC12SHT0_3 |       // 32 ADC12CLK cycles
                ADC12MSC    |       // Multiple sample
                ADC12ON;            // Liga AD

    ADC12CTL1 = ADC12SHP      |     // Sampling timer
                ADC12CONSEQ_1;      // Sequencia de canais

    ADC12CTL2 = ADC12TCOFF |        // Desliga sensor de temperatura
                ADC12RES_2;

    ADC12MCTL1 = ADC12EOS  |        // End of sequence
                 ADC12INCH_1;       // Canal A1

    ADC12IFG = 0;

    ADC12CTL0 |= ADC12ENC;          // Habilita conversoes
}

void leValoresJoystick(uint16_t *eixoX, uint16_t *eixoY) {
    ADC12CTL0 &= ~ADC12SC;                  // Gera um pulso para
    ADC12CTL0 |=  ADC12SC;                  // iniciar a conversao

    while(!(ADC12IFG & ADC12IFG1));

    *eixoY = ADC12MEM0;
    *eixoX = ADC12MEM1;
}
