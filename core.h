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
    N,      // Negro
    // I - CIAN
    I,
    I_OSCURO,
    I_CLARO,
    // O - AMARILLO
    O,
    O_OSCURO,
    O_CLARO,
    // T - PÚRPURA
    T,
    T_OSCURO,
    T_CLARO,
    // S - VERDE
    S,
    S_OSCURO,
    S_CLARO,
    // Z - ROJO
    Z,
    Z_OSCURO,
    Z_CLARO,
    // J - AZUL
    J,
    J_OSCURO,
    J_CLARO,
    // L - NARANJA
    L,
    L_OSCURO,
    L_CLARO,
    // INTERFAZ Y BORDES
    BRD,
    BRD_OSCURO,
    BRD_CLARO,

    W,                      // Blanco estático
    TR                      // Transparente
} eColorTetris;

typedef enum {
    BLOQUE,
    PLANO,
    BORDE,
    FONDO
} eTexturas;


#define ORDEN 4

//MACROS
#define MAX_NOMBRE 3
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
#define CANT_PROXIMAS 4
#define CONFIG_FILE "config.txt"


#endif // CORE_H_INCLUDED
