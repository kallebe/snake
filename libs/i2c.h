#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>

#define SDA                     4,1         // Dados
#define SCL                     4,2         // Clock

#define TCLK4                   2,us        // TCLK/4 => f = @125kHz

// BitBanging
void i2cSendByteBB(uint8_t byte);

#endif // _I2C_H
