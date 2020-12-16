#include <msp430.h> 
#include <stdint.h>
#include "libs/adc.h"
#include "libs/gpio.h"
#include "libs/timer.h"
#include "libs/lcd.h"
#include "libs/snake.h"
#include "libs/i2c.h"

uint8_t snakeSize       = 1;
enum direcao_t snakeDir = direita;

Posicao snake[MAX_SIZE];
Posicao comida;

// Joystick
volatile uint16_t eixoX = 0;
volatile uint16_t eixoY = 0;

void main() {
	WDTCTL = WDTPW | WDTHOLD;	            // Stop watchdog timer

	// Inicializa ADC
	adcInit();

	// Inicializacao do LCD
    pinMode(LCD_RST, output);
    pinMode(LCD_CS,  output);
    lcdInit();
    lcdSetGraphicMode(LCD_GRAPHIC_ON);

    // Inicializa jogo
    inicializaJogo(snake, &snakeSize, &snakeDir, &comida);

    // Desenha cobra
    desenhaCobra(snake, snakeSize, comida);
    desenhaComida(comida);

    while (1) {
        moverCobra(snake, snakeSize, snakeDir);
        desenhaCobra(snake, snakeSize, comida);
        colisoes(snake, &snakeSize, &comida, &snakeDir);
        desenhaComida(comida);

        leValoresJoystick(&eixoX, &eixoY);

        // Define direcao da cobra
        if (snakeDir == direita || snakeDir == esquerda) {
            if (eixoY > 0xAAA)
                snakeDir = cima;
            else if (eixoY < 0x555)
                snakeDir = baixo;
        } else {
            if (eixoX > 0xAAA)
                snakeDir = direita;
            else if (eixoX < 0x555)
                snakeDir = esquerda;
        }
    }

    lcdSetGraphicMode(LCD_GRAPHIC_OFF);

    __low_power_mode_0();
}
