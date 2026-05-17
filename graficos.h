#ifndef GRAFICOS_H_INCLUDED
#define GRAFICOS_H_INCLUDED

#include "core.h"
#include "tdatablero.h"
#include "funciones.h"
void DibujarCelda(uint16_t X, uint16_t Y, uint8_t color, uint8_t TAMANIO);
void DibujarTablero(matrix* m, uint16_t X, uint16_t Y);
void DibujarPieza(PiezaActiva* p);
void DibujarCaracter(char c, uint16_t X, uint16_t Y, uint8_t color);
#endif // GRAFICOS_H_INCLUDED
