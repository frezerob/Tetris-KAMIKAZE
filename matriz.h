#ifndef MATRIZ_H_INCLUDED
#define MATRIZ_H_INCLUDED

#include "core.h"
#include <stdint.h>
typedef struct{
    int** mat;
    uint8_t fil;
    uint8_t col;
}matriz;

int8_t MatrizIniciar(matriz* m, uint8_t fil, uint8_t col);
void MatrizMostrar(matriz* m);
int8_t MatrizFilaLlena(matriz* m);
void MatrizVaciarFil(matriz *m, uint8_t row);
void MatrizDespFil(matriz* m, uint8_t row);

#endif // MATRIZ_H_INCLUDED


