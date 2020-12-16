#ifndef _LCD_H
#define _LCD_H

#include "gpio.h"
#include <stdint.h>

// Portas F5529
#define LCD_CS                  2,6         // Chip Select
#define LCD_RST                 3,7         // Reset

// Byte a ser enviado antes de comando/dado
#define LCD_CMD                 0xF8        // Escreve comando
#define LCD_DATA                0xFA        // Escreve dado

// Comandos LCD
#define LCD_CLEAR_SCREEN        0x01        // Limpa tela do LCD
#define LCD_ADDRESS_RESET       0x02        // Reseta Address Counter
#define LCD_BASIC_FUNCTION      0x30        // Basic instruction set
#define LCD_EXTEND_FUNCTION     0x34        // Extended instruction set

// Macros
#define LCD_GRAPHIC_ON          1
#define LCD_GRAPHIC_OFF         0

void lcdInit();                                 // Inicializa LCD
void lcdWriteByte(uint8_t type, uint8_t byte);  // Escreve um byte no LCD
void lcdWrite(char *str);                       // Escreve uma string

void lcdSetGraphicMode(uint8_t graphicMode);    // Seta Modo grafico
void lcdClearGraphicMem();                      // Limpa LCD em modo grafico

#endif
