#ifndef _SNAKE_H
#define _SNAKE_H

#include <stdint.h>

enum direcao_t { direita, baixo, esquerda, cima };

typedef struct pos {
    uint8_t x;
    uint8_t y;
} Posicao;

#define MAX_SIZE        255     // Tamanho maximo da cobra
#define SNAKE_WIDTH     4       // Espessura da cobra

void inicializaJogo(Posicao *snake, uint8_t *size, enum direcao_t *dir, Posicao *comida);
void desenhaBorda();
uint16_t borderMask(uint8_t x);
void desenhaSegCobra(uint8_t addrX, uint8_t addrY, uint16_t body);
void desenhaCobra(Posicao *snake, uint8_t size, Posicao comida);
void moverCobra(Posicao *snake, uint8_t size, enum direcao_t dir);
void geraComida(Posicao *comida, Posicao *snake);
void desenhaComida(Posicao comida);
void colisoes(Posicao *snake, volatile uint8_t *size, Posicao *comida, enum direcao_t *dir);

#endif
