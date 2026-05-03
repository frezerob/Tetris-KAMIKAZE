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
            gbt_dibujar_pixel(PosX+j,PosY+i,m->mat[i][j]);
        }
    }
}

void DibujarPieza(PiezaActiva* p)
{


    for(uint8_t i=0; i<ORDEN;i++){
        for(uint8_t j=0; j<ORDEN; j++){
            if (p->forma[p->rotacion][i*4+j]!=TR)
                gbt_dibujar_pixel(p->posX+j ,p->posY+i , p->forma[p->rotacion][i*4+j]);
        }
    }
}
