#include <stdint.h>
#include "lcd.h"
#include "gpio.h"
#include "i2c.h"
#include "timer.h"

void lcdInit() {
    // Reseta LCD
    digitalWrite(LCD_RST, 0);
    wait(200, ms);
    digitalWrite(LCD_RST, 1);

    // Ativa escrita no escravo
    digitalWrite(LCD_CS, 1);

    // Inicializa LCD
    lcdWriteByte(LCD_CMD, LCD_BASIC_FUNCTION);
    lcdWriteByte(LCD_CMD, LCD_CLEAR_SCREEN);
    lcdWriteByte(LCD_CMD, 0x06);            // Cursor move para a direita
    lcdWriteByte(LCD_CMD, 0x0C);            // Display On, Cursor Off, Blink Off
}

// type pode ser LCD_CMD  -> comando
//            ou LCD_DATA -> dados
void lcdWriteByte(uint8_t type, uint8_t byte) {
    i2cSendByteBB(type);                    // Escreve comando ou dado
    i2cSendByteBB(byte & 0xF0);             // Escreve parte mais significativa
    i2cSendByteBB((byte << 4) & 0xF0);      // Escreve parte menos significativa
}

void lcdWrite(char *str) {
    while (*str) {
        lcdWriteByte(LCD_DATA, *str++);
    }
}

// Seta graphic mode quando graphic = LCD_GRAPHIC_ON
// ou text mode quando      graphic = LCD_GRAPHIC_OFF
void lcdSetGraphicMode(uint8_t graphicMode) {
    if (graphicMode == LCD_GRAPHIC_ON) {
        lcdWriteByte(LCD_CMD, LCD_EXTEND_FUNCTION);
    } else {
        lcdWriteByte(LCD_CMD, LCD_BASIC_FUNCTION);
    }

    lcdWriteByte(LCD_CMD, LCD_EXTEND_FUNCTION | 0x02);
}

void lcdClearGraphicMem() {
    uint8_t x, y;
    for (x = 0; x < 16; x++) {
        for (y = 0; y < 32; y++) {
            lcdWriteByte(LCD_CMD, 0x80 | y);
            lcdWriteByte(LCD_CMD, 0x80 | x);
            lcdWriteByte(LCD_DATA, 0x00);         // Escreve 0 na coordenada vertical
            lcdWriteByte(LCD_DATA, 0x00);         // Escreve 0 na coordenada horizontal
        }
    }
}
