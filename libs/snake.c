#include <msp430.h>
#include <time.h>
#include <stdlib.h>
#include "snake.h"
#include "lcd.h"

void inicializaJogo(Posicao *snake, uint8_t *size, enum direcao_t *dir, Posicao *comida) {
    lcdClearGraphicMem();
    desenhaBorda();

    snake[0].x = 6;
    snake[0].y = 28;
    snake[1].x = 5;                         // Caminho 'anterior' da cobra
    snake[1].y = 28;
    *size = 1;
    *dir  = direita;

    geraComida(comida, snake);
}

void desenhaBorda() {
    uint8_t x = 0, y = 0;

    for (y = 0; y < 32; y++) {
        // Borda esquerda superior
        lcdWriteByte(LCD_CMD, 0x80 | y);
        lcdWriteByte(LCD_CMD, 0x80 | 0);
        lcdWriteByte(LCD_DATA, 0xC0);
        lcdWriteByte(LCD_DATA, 0x00);

        // Borda esquerda inferior
        lcdWriteByte(LCD_CMD, 0x80 | 31 - y);
        lcdWriteByte(LCD_CMD, 0x88 | 0);
        lcdWriteByte(LCD_DATA, 0xC0);
        lcdWriteByte(LCD_DATA, 0x00);

        // Borda direita superior
        lcdWriteByte(LCD_CMD, 0x80 | y);
        lcdWriteByte(LCD_CMD, 0x80 | 7);
        lcdWriteByte(LCD_DATA, 0x00);
        lcdWriteByte(LCD_DATA, 0x03);

        // Borda direita inferior
        lcdWriteByte(LCD_CMD, 0x80 | 31 - y);
        lcdWriteByte(LCD_CMD, 0x88 | 7);
        lcdWriteByte(LCD_DATA, 0x00);
        lcdWriteByte(LCD_DATA, 0x03);
    }

    for (x = 0; x < 8; x++) {
        // Borda superior
        lcdWriteByte(LCD_CMD, 0x80 | 0);
        lcdWriteByte(LCD_CMD, 0x80 | x);
        lcdWriteByte(LCD_DATA, 0xFF);
        lcdWriteByte(LCD_DATA, 0xFF);

        lcdWriteByte(LCD_CMD, 0x80 | 1);
        lcdWriteByte(LCD_CMD, 0x80 | x);
        lcdWriteByte(LCD_DATA, 0xFF);
        lcdWriteByte(LCD_DATA, 0xFF);

        // Borda Inferior
        lcdWriteByte(LCD_CMD, 0x80 | 31);
        lcdWriteByte(LCD_CMD, 0x88 | x);
        lcdWriteByte(LCD_DATA, 0xFF);
        lcdWriteByte(LCD_DATA, 0xFF);

        lcdWriteByte(LCD_CMD, 0x80 | 30);
        lcdWriteByte(LCD_CMD, 0x88 | x);
        lcdWriteByte(LCD_DATA, 0xFF);
        lcdWriteByte(LCD_DATA, 0xFF);
    }
}

void desenhaSegCobra(uint8_t addrX, uint8_t addrY, uint16_t body) {
    lcdWriteByte(LCD_CMD, 0x80 | addrY);
    lcdWriteByte(LCD_CMD, 0x80 | addrX);
    lcdWriteByte(LCD_DATA, body >> 8);
    lcdWriteByte(LCD_DATA, body);

    lcdWriteByte(LCD_CMD, 0x80 | addrY + 1);
    lcdWriteByte(LCD_CMD, 0x80 | addrX);
    lcdWriteByte(LCD_DATA, body >> 8);
    lcdWriteByte(LCD_DATA, body);

    lcdWriteByte(LCD_CMD, 0x80 | addrY + 2);
    lcdWriteByte(LCD_CMD, 0x80 | addrX);
    lcdWriteByte(LCD_DATA, body >> 8);
    lcdWriteByte(LCD_DATA, body);

    lcdWriteByte(LCD_CMD, 0x80 | addrY + 3);
    lcdWriteByte(LCD_CMD, 0x80 | addrX);
    lcdWriteByte(LCD_DATA, body >> 8);
    lcdWriteByte(LCD_DATA, body);
}

void desenhaCobra(Posicao *snake, uint8_t size, Posicao comida) {
    uint8_t x, y, prevX, offset;

    uint8_t xComida = comida.x >> 2;
    offset = comida.x - (xComida * 4);
    uint16_t maskComida = 0xF000 >> (SNAKE_WIDTH * offset);
    uint16_t mask, border;

    uint8_t i;

    x = snake[size].x >> 2;
    y = snake[size].y;

    border = borderMask(x);

    if (x == xComida && y == comida.y)
        desenhaSegCobra(x, y, maskComida | border);
    else
        desenhaSegCobra(x, y, border);

    for (i = size - 1; i < size; i--) {
        x     = snake[i].x >> 2;
        y     = snake[i].y;
        prevX = x;
        mask  = 0x0000;

        while (i < size && prevX == x && y == snake[i].y) {
            x       = snake[i].x >> 2;
            y       = snake[i].y;
            offset  = snake[i].x - (x * 4);
            mask   |= 0xF000 >> (SNAKE_WIDTH * offset);

            i--;
            prevX = x;
            x     = snake[i].x >> 2;
        }

        if (i < size && (x != prevX || y != snake[i].y))
            i++;

        border = borderMask(prevX);
        if (x == xComida && y == comida.y)
            desenhaSegCobra(prevX, y, border | maskComida | mask);
        else
            desenhaSegCobra(prevX, y, border | mask);
    }
}

uint16_t borderMask(uint8_t x) {
    if (x == 0 || x == 8)
        return 0xC000;

    if (x == 7 || x == 15)
        return 0x0003;

    return 0;
}

void moverCobra(Posicao *snake, uint8_t size, enum direcao_t dir) {
    uint8_t i, y;

    // Atualiza corpo da cobra
    for (i = size; i > 0; i--)
        snake[i] = snake[i-1];

    // Atualiza cabeca
    if (dir == direita)
        snake->x += 1;
    if (dir == baixo) {
        y = snake->y + SNAKE_WIDTH;
        if (y > 31) {
            snake->y = 32 - y;
            snake->x += 32;
        } else {
            snake->y = y;
        }
    }
    if (dir == esquerda)
        snake->x -= 1;
    if (dir == cima) {
        y = snake->y - SNAKE_WIDTH;
        if (y > 64) {
            snake->y = 28 - snake->y;
            snake->x -= 32;
        } else {
            snake->y = y;
        }
    }
}

void desenhaComida(Posicao comida) {
    uint8_t x          = comida.x >> 2;
    uint8_t y          = comida.y;
    uint8_t offset     = comida.x - (x * 4);
    uint16_t border    = borderMask(x);
    uint16_t topBottom = border | 0x6000 >> (SNAKE_WIDTH * offset);
    uint16_t rightLeft = border | 0xF000 >> (SNAKE_WIDTH * offset);

    lcdWriteByte(LCD_CMD, 0x80 | y);
    lcdWriteByte(LCD_CMD, 0x80 | x);
    lcdWriteByte(LCD_DATA, topBottom >> 8);
    lcdWriteByte(LCD_DATA, topBottom);

    lcdWriteByte(LCD_CMD, 0x80 | y + 1);
    lcdWriteByte(LCD_CMD, 0x80 | x);
    lcdWriteByte(LCD_DATA, rightLeft >> 8);
    lcdWriteByte(LCD_DATA, rightLeft);

    lcdWriteByte(LCD_CMD, 0x80 | y + 2);
    lcdWriteByte(LCD_CMD, 0x80 | x);
    lcdWriteByte(LCD_DATA, rightLeft >> 8);
    lcdWriteByte(LCD_DATA, rightLeft);

    lcdWriteByte(LCD_CMD, 0x80 | y + 3);
    lcdWriteByte(LCD_CMD, 0x80 | x);
    lcdWriteByte(LCD_DATA, topBottom >> 8);
    lcdWriteByte(LCD_DATA, topBottom);
}

void geraComida(Posicao *comida, Posicao *snake) {
    srand((unsigned int) time(NULL));

    do {
        comida->x = (rand() % 60) + 2;
        comida->y = (rand() % 7) * 4 + 4;
    } while (comida->x != snake->x);
}

void colisoes(Posicao *snake, volatile uint8_t *size, Posicao *comida, enum direcao_t *dir) {
    // Comer alimento
    if (snake->x == comida->x && snake->y == comida->y) {
        snake[*size] = snake[*size-1];
        *size = *size + 1;

        geraComida(comida, snake);
    }

    // Colisao com bordas do jogo
    else if (snake->x == 0 || snake->x == 31 || snake->x == 32 || snake->x == 63 || (snake->x < 31 && snake->y == 0) || (snake->x > 31 && snake->y == 28)) {
        inicializaJogo(snake, size, dir, comida);       // Reinicia o jogo
    }

    // Colisao com proprio corpo
    else {
        uint8_t i;
        for (i = *size - 1; i > 0; i--) {
            if (snake[i].x == snake->x && snake[i].y == snake->y) {
                inicializaJogo(snake, size, dir, comida);
                break;
            }
        }
    }
}
