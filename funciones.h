#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "core.h"


typedef struct {
    int (*forma)[16];
    int rotacion;
    int posX;
    int posY;
} PiezaActiva;

// Prototipos de funciones
void semilla();
int generarPiezaAleatoria();
void tipoPieza(PiezaActiva* pieza, int tipoSeleccionado);
int8_t PiezaPuedeRotar(PiezaActiva* p);
void PiezaMoverAbajo(PiezaActiva* p);
void PiezaMoverIzq(PiezaActiva* p);
void PiezaMoverDer(PiezaActiva* p);
void PiezaMoverArriba(PiezaActiva* p);
int8_t PiezaDetectarColision(PiezaActiva* p);
#endif // FUNCIONES_H_INCLUDED
