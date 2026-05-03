#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "matriz.h"

void MatrizMostrar(matriz* m)
{
    for(uint8_t i=0; i<m->fil; i++)
    {
        for(uint8_t j=0; j<m->col; j++)
        {
            printf("[%d]",m->mat[i][j]);
        }
        printf("\n");
    }
}


/*
 * Inicializa la matriz
 * Devuelve 0 Si OK y -1 Si hay error
 */
int8_t MatrizIniciar(matriz* m, uint8_t fil, uint8_t col)
{   m->mat = malloc (fil * sizeof(void*));
    if(m->mat==NULL)
        return INIT_ERR;

    //inicializamos sus punteros

    for(int i = 0; i < fil ; i++)
    {
        m->mat[i]=calloc(col, sizeof(uint8_t));
        if(m->mat[i] == NULL)
            return INIT_ERR;
    }

    m->col = col;
    m->fil = fil;

    return OK;
}

/*
 * Dada una matriz m devuelva la primera fila llena que encuentre
 * Si no encuentra nada devuelve -1
 */
int8_t MatrizFilaLlena(matriz* m)
{
    int flag;
    for(int i=0; i< m->fil ; i++){
        flag=0;
        for(int j=0 ; j< m->col ; j++){
            if(m->mat[i][j] == 0)
                flag=1;
        }
        if(!flag)
            return i;
    }

    return -1;
}

/*
 * Reinicia a 0 una fila de la matriz pasada como parametro
 */
void MatrizVaciarFil(matriz *m, uint8_t row)
{
    for(uint8_t j=0; j<m->col;j++)
    {
        m->mat[row][j]=0;
    }
}


/*
 * Dada una matriz m y una fila row
 * Desplaza la matris hacia abajo y la fila row pasa a ser la primera.
 */
void MatrizDespFil(matriz* m, uint8_t row)
{
    int* aux = m->mat[row];
    for(uint8_t i = row ; i>0 ; i--)
    {
        m->mat[i] = m->mat[i-1];
    }

    m->mat[0] = aux;
}
