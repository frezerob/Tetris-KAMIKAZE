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




#endif // FUNCIONES_H_INCLUDED
