#ifndef GRAFICOS_H_INCLUDED
#define GRAFICOS_H_INCLUDED

#include "core.h"
#include "tdatablero.h"
#include "funciones.h"
void DibujarCelda(uint16_t X, uint16_t Y, uint8_t color, uint8_t TAMANIO, eTexturas Textura);
void DibujarTablero(matrix* m, uint16_t X, uint16_t Y);
void DibujarPieza(PiezaActiva* p);


//TEXTO
void DibujarCaracter(char c, uint16_t X, uint16_t Y,uint8_t color);
void DibujarNumero(char num,uint16_t X, uint16_t Y, uint8_t color);
void DibujarLetra(char c, uint16_t X, uint16_t Y, uint8_t color);
void DibujarTexto(char* texto, uint16_t X, uint16_t Y, uint8_t color);
void DibujarTextoCentrado(char* texto, uint16_t Y, uint8_t color, uint16_t OFFSET_X);
void DibujarTextoCentradoConSombra(char* texto, uint16_t Y, uint8_t color, uint16_t OFFSET_X, uint8_t colorSombra);
void DibujarTextoConSombra(char* texto, uint16_t X, uint16_t Y, uint8_t color, uint8_t colorSombra);
void DibujarTextoEnRecuadro(char* texto, uint16_t centroX, uint16_t centroY, uint8_t colorTexto, uint8_t colorFondo, eTexturas texturaBorde);

void DibujarBitMap(const uint8_t bitmap[], uint16_t X, uint16_t Y, uint8_t color);
void DibujarBitMap8x8(const uint8_t bitmap[8], uint16_t X, uint16_t Y, uint8_t color);
void DibujarBitMap8x16(const uint8_t bitmap[16], uint16_t X, uint16_t Y, uint8_t color);
//
void DibujarPuntaje(int puntaje, uint16_t X, uint16_t Y, uint8_t color);
void DibujarProximaPieza(uint8_t (*forma)[16], uint16_t X, uint16_t Y);
void DibujarFondo();

void DibujarRectangulo(uint16_t X, uint16_t Y, uint16_t ancho_celdas, uint16_t alto_celdas, uint8_t color, eTexturas textura);

void ImprimirMenu(uint8_t opcion, char* opciones_menu[], size_t cant_opciones);

void RenderizarJuego(PiezaActiva *p, matrix *m, int puntaje, int proximas[], char*nombre);
#endif // GRAFICOS_H_INCLUDED
