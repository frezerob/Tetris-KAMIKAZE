#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED


#include "core.h"
#include <stdint.h>

typedef struct{
    uint16_t ANCHO;
    uint16_t ALTO;
    float ESCALA;
    uint8_t TAM_CELDA;
    uint16_t OFFSET_X;
    uint16_t OFFSET_Y;
    float ESCALA_FUENTE;
    int DIFICULTAD;
    int PALETA;
    uint8_t FUENTE;
    ModoJuego MODO;
    uint8_t COL_TABLERO;
    uint8_t FIL_TABLERO;
    uint8_t ultimaPosX;
}TDAconfig;

extern TDAconfig config;

int ConfigCargar(char* archivo);
int ConfigGuardar(char* archivo);
void ConfigAplicarResolucion();

void AplicarConfig(uint16_t res);


#endif // CONFIG_H_INCLUDED
