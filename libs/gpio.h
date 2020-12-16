#ifndef _GPIO_H
#define _GPIO_H

#include <stdint.h>

enum pinModes { input, output, inPullUp, inPullDown };

void    digitalWrite(uint8_t port, uint8_t bit, uint8_t value);
uint8_t digitalRead (uint8_t port, uint8_t bit);
void        pinMode (uint8_t port, uint8_t bit, enum pinModes mode);

#endif // _GPIO_H
