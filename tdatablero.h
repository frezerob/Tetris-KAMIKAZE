#ifndef TDATABLERO_H_INCLUDED
#define TDATABLERO_H_INCLUDED

#include "core.h"
#include <stdint.h>

typedef struct{
    uint8_t** mat;
    int fil;
    int col;
}matrix;

uint8_t MatrizIniciar(matrix* m, int fil, int col);
void MatrizMostrar(matrix* m);
uint8_t MatrizFilCompleta(matrix* m);
void MatrizVaciarFil(matrix *m, uint8_t row);
void MatrizDespFil(matrix* m, uint8_t row);


void PiezaVolcar(matrix* m);
#endif // TDATABLERO_H_INCLUDED
