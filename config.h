#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <stdint.h>

typedef struct{
    uint16_t ANCHO;
    uint16_t ALTO;
    uint8_t ESCALA;
    uint8_t TAM_CELDA;
    uint16_t OFFSET_X;
    uint16_t OFFSET_Y;
    uint8_t ESCALA_FUENTE;
    int DIFICULTAD;
    int PALETA;
}TDAconfig;

extern TDAconfig config;

int ConfigCargar(char* archivo);
int ConfigGuardar(char* archivo);
void ConfigAplicarResolucion();

#endif // CONFIG_H_INCLUDED
