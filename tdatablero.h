#ifndef TDATABLERO_H_INCLUDED
#define TDATABLERO_H_INCLUDED

#include "core.h"
#include <stdint.h>

typedef struct
{
    uint16_t** tab;
}Stablero;

int8_t StableroIniciar(Stablero* tablero);
void StableroMostrar(Stablero* tablero);
int8_t StableroPermutarFil(Stablero* tablero,uint8_t fil_A,uint8_t fil_B);
int8_t StableroFilaLlena(Stablero* tablero, uint8_t fil);
int8_t StableroReiniciarFil(Stablero* tablero, uint8_t fil);
int8_t StableroDesplazarFil(Stablero* tablero, uint8_t fil);
#endif // TDATABLERO_H_INCLUDED
