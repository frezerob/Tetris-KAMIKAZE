#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tdatablero.h"
#include "core.h"

/*
Inicializa el tablero alocando las 25 filas y
las 10 columnas por fila.
Retorna -1 si fallo al iniciar
*/
int8_t StableroIniciar(Stablero* tablero)
{
    tablero->tab = malloc(FIL_TABLERO * sizeof(uint16_t*));
    if(tablero->tab == NULL)
        return INIT_ERROR;

    int i;

    for(i=0;i<FIL_TABLERO;i++){

        tablero->tab[i] = calloc(COL_TABLERO, sizeof(uint16_t));

        if(tablero->tab[i] == NULL)
           return INIT_ERROR;

    }
    return 0;

}

/*
Dada una fila A y una fila B.
Se permutan las filas con el puntero al Stablero
Si los rangos de las filas son erroneos devuelve RANGE_ERROR(-4)
*/
int8_t StableroPermutarFil(Stablero* tablero,uint8_t fil_A,uint8_t fil_B)
{
     if(fil_A>(FIL_TABLERO-1) || fil_B>(FIL_TABLERO-1))
        return RANGE_ERROR;
     if(fil_A==fil_B){
        printf("No se puede permutar la misma fila.");
        return 0;
     }

     uint16_t* temp = tablero->tab[fil_A];
     tablero->tab[fil_A] = tablero->tab[fil_B];
     tablero->tab[fil_B] = temp;

     return 0;
}

/*
Muestra el tablero dada un puntero a Stablero
*/
void StableroMostrar(Stablero* tablero)
{
    uint8_t f,c;
    for(f=0;f<FIL_TABLERO;f++){
        for(c=0;c<COL_TABLERO;c++){
            printf("%d",tablero->tab[f][c]);
        }
        printf("\n");
    }
}


/*
En base a una fila y un tablero
devuelve si la fila está llena
*/
int8_t StableroFilaLlena(Stablero* tablero, uint8_t fil)
{
    if(fil>(FIL_TABLERO-1)){
        printf("Valor fuera de rango.");
        return RANGE_ERROR;
    }

    uint8_t i;
    for(i=0;i<COL_TABLERO;i++)
        if(tablero->tab[fil][i] == 0)
            return FALSE;
    return TRUE;
}

int8_t StableroReiniciarFil(Stablero* tablero, uint8_t fil)
{
    if(fil>(FIL_TABLERO-1))
        return RANGE_ERROR;
    uint8_t i;
    for(i=0;i<(FIL_TABLERO-1);i++)
        tablero->tab[fil][i]=0;
    return OK;
}

int8_t StableroDesplazarFil(Stablero* tablero, uint8_t fil)
{
    uint8_t i;

    uint16_t* aux = tablero->tab[fil];

    for(i=fil;i>0;i--)
    {
        tablero->tab[i] = tablero->tab[i-1];
    }

    tablero->tab[0] = aux;

    return OK;
}

