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
}TDAconfig;

extern TDAconfig config;

#endif // CONFIG_H_INCLUDED
