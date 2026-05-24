#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "core.h"
#include "tdatablero.h"




extern uint8_t (*FORMAS[7])[16];

// Prototipos de funciones
void semilla();
uint8_t generarPiezaAleatoria();
void tipoPieza(PiezaActiva* pieza, uint8_t tipoSeleccionado, matrix* m);
int8_t PiezaPuedeRotar(PiezaActiva* p);
void PiezaMoverAbajo(PiezaActiva* p);
void PiezaMoverIzq(PiezaActiva* p);
void PiezaMoverDer(PiezaActiva* p);
void PiezaMoverArriba(PiezaActiva* p);
void PiezaRotarDerecha(PiezaActiva* p);
int8_t PiezaDetectarColision(PiezaActiva* p, matrix* m);
void PiezaRotarIzquierda(PiezaActiva* p);
int RecalcularVelocidad(int velInicial, int piezas);
uint16_t VelocidadSegunDificultad(int dificultad);
int MultiplicadorPuntos(int velActual);
#endif // FUNCIONES_H_INCLUDED
