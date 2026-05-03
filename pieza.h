#ifndef PIEZA_H_INCLUDED
#define PIEZA_H_INCLUDED

#include "core.h"
#include "matriz.h"

typedef struct{
    char tipo;
    matriz m;
    int x;
    int y;
    int rotation;
}pieza;

int8_t PiezaIniciar(pieza* p, uint8_t orden);

#endif // PIEZA_H_INCLUDED
