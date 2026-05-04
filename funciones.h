#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "core.h"
#include "tdatablero.h"





// Prototipos de funciones
void semilla();
int generarPiezaAleatoria();
void tipoPieza(PiezaActiva* pieza, int tipoSeleccionado);
int8_t PiezaPuedeRotar(PiezaActiva* p);
void PiezaMoverAbajo(PiezaActiva* p);
void PiezaMoverIzq(PiezaActiva* p);
void PiezaMoverDer(PiezaActiva* p);
void PiezaMoverArriba(PiezaActiva* p);
void PiezaRotarDerecha(PiezaActiva* p);
int8_t PiezaDetectarColision(PiezaActiva* p, matrix* m);
void PiezaRotarIzquierda(PiezaActiva* p);
#endif // FUNCIONES_H_INCLUDED
