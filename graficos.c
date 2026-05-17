#include <stdio.h>
#include <stdlib.h>
#include "graficos.h"
#include "GBT/gbt.h"
#include "funciones.h"
#include "core.h"

void DibujarTablero(matrix* m, uint16_t X, uint16_t Y){

    uint16_t PosX = X + OFFSET;
    uint16_t PosY = Y + OFFSET;
    for(uint8_t i=0; i<m->fil;i++){
        for(uint8_t j=0; j<m->col; j++){
            DibujarCelda(PosX+j*config.TAM_CELDA,PosY+i*config.TAM_CELDA,m->mat[i][j]);
        }
    }
}

void DibujarPieza(PiezaActiva* p)
{
    for(uint8_t i = 0; i < ORDEN; i++){
        for(uint8_t j = 0; j < ORDEN; j++){
            if(p->forma[p->rotacion][i*ORDEN+j] != TR)
                DibujarCelda(OFFSET + (p->posX + j ) * config.TAM_CELDA,  // offset solo acá
                                  OFFSET + (p->posY + i)  * config.TAM_CELDA,
                                  p->forma[p->rotacion][i * ORDEN + j]);
        }
    }
}

void DibujarCelda(uint16_t X, uint16_t Y, uint8_t color)
{
    uint16_t LIM_Y = Y + config.TAM_CELDA, LIM_X = X + config.TAM_CELDA;
    for(uint16_t f = Y; f<LIM_Y; f++)
        for(uint16_t c = X; c< LIM_X; c++)
            gbt_dibujar_pixel(c,f,color);
}
