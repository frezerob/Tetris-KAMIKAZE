#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"
#include "pieza.h"

int8_t PiezaIniciar(pieza* p, uint8_t orden)
{
    const char pieces[7]={'I','J','L','O','S','T','Z'};

    if(MatrizIniciar(&p->m,orden,orden) == -1)
        return INIT_ERR;

    uint8_t Npiece = (int)rand()%7;
    char piece = pieces[Npiece];

    p->tipo = piece;
    p->rotation = 0;

    for(uint8_t i = 0; i<orden; i++)
    {
        for(uint8_t j = 0; j < orden; j++)
        {
            p->m.mat[i][j] = piezas[Npiece][0][i][j];
        }
    }
    return OK;
}
