#ifndef GRAFICOS_H_INCLUDED
#define GRAFICOS_H_INCLUDED

#include "core.h"
#include "tdatablero.h"
#include "funciones.h"
void DibujarCelda(uint16_t X, uint16_t Y, uint8_t color, uint8_t TAMANIO, eTexturas Textura);
void DibujarTablero(matrix* m, uint16_t X, uint16_t Y);
void DibujarPieza(PiezaActiva* p);
void DibujarCaracter(char c, uint16_t X, uint16_t Y,uint8_t color);
void DibujarNumero(char num,uint16_t X, uint16_t Y, uint8_t color);
void DibujarLetra(char c, uint16_t X, uint16_t Y, uint8_t color);
void DibujarTexto(char* texto, uint16_t X, uint16_t Y, uint8_t color);
void DibujarTextoCentrado(char* texto, uint16_t Y, uint8_t color, uint16_t OFFSET_X);
void DibujarPuntaje(int puntaje, uint16_t X, uint16_t Y, uint8_t color);
void DibujarProximaPieza(uint8_t (*forma)[16], int X, int Y);
void DibujarFondo();

void DibujarRectangulo(uint16_t X, uint16_t Y, uint16_t ancho_celdas, uint16_t alto_celdas, uint8_t color, eTexturas textura);
void ImprimirMenu(uint8_t opcion, char* opciones_menu[], size_t cant_opciones);
#endif // GRAFICOS_H_INCLUDED
