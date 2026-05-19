#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED
//LIBRERIAS
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "GBT/gbt.h"
#include "fuente8x8.h"
#include "config.h"
#include <string.h>

typedef struct {
    uint8_t (*forma)[16];
    int rotacion;
    int posX;
    int posY;
} PiezaActiva;

typedef enum {
    N,   // Negro
    I,   // Cian
    I_OSCURO,   // Cian
    O,   // Amarillo
    O_OSCURO,   // Amarillo
    T,   // Púrpura
    T_OSCURO,   // Púrpura
    S,   // Verde
    S_OSCURO,   // Verde
    Z,   // Rojo
    Z_OSCURO,   // Rojo
    J,   // Azul
    J_OSCURO,   // Azul
    L,   // Naranja
    L_OSCURO,   // Naranja
    BRD, // Borde
    W,   // Blanco
    TR   // Transparente
} eColorTetris;

typedef enum {
    BLOQUE,
    PLANO,
    BORDE,
    FONDO
} eTexturas;
#define ORDEN 4

//MACROS
#define CANT_TEXTURAS 4
#define TITULO "Primera Prueba"
//MACROS TABLERO
#define FIL_TABLERO 20
#define COL_TABLERO 10
#define TAM_FUENTE8X8 8

//ESTADOS
#define TRUE 1
#define FALSE 0
#define OK 0
#define INIT_ERR -1
#define COLOR_ERR -2
#define WINDOW_ERR -3
#define RANGE_ERR -4
#define TEMPO_ERR -5
#define FIN 0
#define CANT_PROXIMAS 5
#define CONFIG_FILE "config.txt"


#endif // CORE_H_INCLUDED
