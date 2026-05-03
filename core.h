#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED
//LIBRERIAS
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GBT/gbt.h"

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
#define ANCHO 14
#define ALTO 29
#define ESCALA 25
#define TILE_SIZE 10 // Tamaño de la tile

//MACROS TABLERO
#define FIL_TABLERO 27
#define COL_TABLERO 12
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
