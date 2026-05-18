#ifndef FUENTE8X8_H_INCLUDED
#define FUENTE8X8_H_INCLUDED

#include <stdint.h>

/*
* @brief Fuentes 8x8px. La ultima fila se deja en 0 para ayudar a la legibilidad.
*
*
*
*
*/
extern const uint8_t fuente8x8[26][8];
extern const uint8_t fuente8x8_num[10][8];


uint16_t CalcularAnchoTexto(char* texto);

#endif // FUENTE8X8_H_INCLUDED
