#include "tdatablero.h"
#include "core.h"
#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"
#include "config.h"
void MatrizMostrar(matrix* m)
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

uint8_t MatrizIniciar(matrix* m, int fil, int col)
{   m->mat = malloc (fil * sizeof(void*));
    if(m->mat==NULL)
        return INIT_ERR;

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

void MatrizVaciarFil(matrix *m, uint8_t row)
{
    for(uint8_t j=0; j<m->col;j++)
    {
        m->mat[row][j]=0;
    }
}
/**
 *
 */
void MatrizDespFil(matrix* m, uint8_t row)
{
    uint8_t* aux = m->mat[row];
    for(uint8_t i = row ; i>0 ; i--)
    {
        m->mat[i] = m->mat[i-1];
    }

    m->mat[0] = aux;
}
/**
 * Toma como argumento un puntero a matriz m y un puntero a la pieza actual
 * Vuelca la pieza en la matriz para volverla estática
 */
void PiezaVolcar(matrix* m, PiezaActiva* p)
{
    for(uint8_t i = 0; i < ORDEN; i++){
        for(uint8_t j = 0; j < ORDEN; j++){
            int color = p->forma[p->rotacion][i*ORDEN+j];
            if(color == TR)
                continue;

            int fila = p->posY + i;
            int col  = p->posX + j;

            if(fila >= 0 && fila < config.FIL_TABLERO)
            {
                if(config.MODO == DELUXE)
                {
                    col = (col % config.COL_TABLERO + config.COL_TABLERO) % config.COL_TABLERO;
                    m->mat[fila][col] = color;
                }
                else
                {
                    if(col >= 0 && col < config.COL_TABLERO)
                    {
                        m->mat[fila][col] = color;
                    }
                }
            }
        }
    }
}

/**
 * Toma como argumento un puntero a matriz
 * Devuelva la cantidad de filas completas
 */
int8_t MatrizFilCompleta(matrix* m)
{
    int8_t llena = 1;
    for(int8_t i = m->fil - 1; i >= 0; i--){
            llena = 1; // de abajo hacia arriba
        for(int8_t j = 0; j < m->col; j++){
            if(m->mat[i][j] == N){
                llena = 0;
                break;
            }
        }
        if(llena) return i;
    }
    return -1;
}

/**
 * Toma como argumento un puntero a matrix y una fila
 * Elimina una fila completa (setea a Negro = 0)
 */
void MatrizEliminarFila(matrix* m, int row)
{
    for(int j = 0; j < m->col; j++)
        m->mat[row][j] = N;
    MatrizDespFil(m,row);
}

/**
 * Toma como argumento un puntero a matrix para eliminar las filas completas
 */
int EliminarFilasCompletas(matrix* m)
{
    int filasEliminadas = 0;
    int filaCompleta;
    while((filaCompleta = MatrizFilCompleta(m)) != -1){
        MatrizEliminarFila(m, filaCompleta);
        filasEliminadas++;
    }
    return filasEliminadas;
}

/**
 * Toma como argumento un puntero a matrix para eliminar las filas completas
 * Devuelve el puntaje 100 puntos por fila mas un bonus por completar mas de una
 */
int EliminarFilasCompletasConPuntaje(matrix* m)
{
    uint8_t filasEliminadas = 0;

    int8_t filaCompleta;
    while((filaCompleta = MatrizFilCompleta(m)) != -1) {
        MatrizEliminarFila(m, filaCompleta);
        filasEliminadas++;
    }
    return filasEliminadas > 0 ? filasEliminadas*100 + (filasEliminadas-1)*100 : 0;
}

/**
 * Toma como argumento un puntero a matrix para liberar la memoria
 */
void MatrizBorrar(matrix *m)
{
    for(uint8_t i = 0; i < m->fil; i++)
        free(m->mat[i]);
    free(m->mat);
}
