#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED
//LIBRERIAS
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GBT/gbt.h"
#include "config.h"

typedef struct {
    uint8_t (*forma)[16];
    int rotacion;
    int posX;
    int posY;
} PiezaActiva;

typedef enum {
    N,   // Negro
    I,   // Cian
    O,   // Amarillo
    T,   // Púrpura
    S,   // Verde
    Z,   // Rojo
    J,   // Azul
    L,   // Naranja
    BRD, // Borde
    W,   // Blanco
    TR   // Transparente
} eColorTetris;

#define ORDEN 4

//MACROS
#define TITULO "Primera Prueba"

//MACROS TABLERO
#define FIL_TABLERO 25
#define COL_TABLERO 10
#define OFFSET 1
//ESTADOS
#define TRUE 1
#define FALSE 0
#define OK 0
#define INIT_ERR -1
#define COLOR_ERR -2
#define WINDOW_ERR -3
#define RANGE_ERR -4
#define TEMPO_ERR -5
#endif // CORE_H_INCLUDED
