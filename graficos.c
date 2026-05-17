#include <stdio.h>
#include <stdlib.h>
#include "graficos.h"
#include "GBT/gbt.h"
#include "funciones.h"
#include "core.h"

void DibujarTablero(matrix* m, uint16_t X, uint16_t Y){

    uint16_t PosX = X + config.OFFSET_X;
    uint16_t PosY = Y + config.OFFSET_Y;
    for(uint8_t i=0; i<m->fil;i++){
        for(uint8_t j=0; j<m->col; j++){
            DibujarCelda(PosX+j*config.TAM_CELDA,PosY+i*config.TAM_CELDA,m->mat[i][j],config.TAM_CELDA);
        }
    }
}

void DibujarPieza(PiezaActiva* p)
{
    for(uint8_t i = 0; i < ORDEN; i++){
        for(uint8_t j = 0; j < ORDEN; j++){
            if(p->forma[p->rotacion][i*ORDEN+j] != TR)
                DibujarCelda(config.OFFSET_X + (p->posX + j ) * config.TAM_CELDA,  // offset solo acá
                                  config.OFFSET_Y + (p->posY + i)  * config.TAM_CELDA,
                                  p->forma[p->rotacion][i * ORDEN + j],
                                  config.TAM_CELDA
                                  );
        }
    }
}

void DibujarCelda(uint16_t X, uint16_t Y, uint8_t color, uint8_t TAMANIO)
{
    uint16_t LIM_Y = Y + TAMANIO, LIM_X = X + TAMANIO;
    for(uint16_t f = Y; f<LIM_Y; f++)
        for(uint16_t c = X; c< LIM_X; c++)
            gbt_dibujar_pixel(c,f,color);
}


void DibujarBitMap8x8(const uint8_t bitmap[8], uint16_t X, uint16_t Y, uint8_t color){


    uint8_t bits;
    for(int8_t f=0; f<TAM_FUENTE8X8; f++){

        bits =  bitmap[f];


        for(int8_t c=0; c<TAM_FUENTE8X8; c++){
            if(bits & (1 << (7 - c)))
            {
                DibujarCelda(
                             X + c * config.ESCALA_FUENTE,
                             Y + f * config.ESCALA_FUENTE,
                             color,
                             config.ESCALA_FUENTE
                             );
            }
        }
    }
}



void DibujarLetra(char c, uint16_t X, uint16_t Y, uint8_t color)
{
    if(c<'A' || c>'Z')
        return;
    // 'A' representaria ese offset de 65 que tiene e ASCII
    uint8_t indice = c - 'A';
    DibujarBitMap8x8(fuente8x8[indice],X,Y,color);

}

void DibujarNumero(char num,uint16_t X, uint16_t Y, uint8_t color){
    if (num<'0' || num>'9')
        return;

    uint8_t indice = num - '0';

    DibujarBitMap8x8(fuente8x8_num[indice],X,Y,color);

}


void DibujarCaracter(char c, uint16_t X, uint16_t Y,uint8_t color)
{
    if(c >= 'A' && c<='Z')
        DibujarLetra(c,X,Y,color);
    else if(c>='0' || c<='9')
        DibujarNumero(c,X,Y,color);
}


void DibujarTexto(char* texto, uint16_t X, uint16_t Y, uint8_t color)
{
    while(*texto)
    {

        DibujarCaracter(*texto,X,Y,color);

        X += TAM_FUENTE8X8 * config.ESCALA_FUENTE + 1;

        texto++;

    }
}

void ImprimirMenu()
{
    gbt_borrar_backbuffer(BRD);

    uint16_t TITULO_Y = config.ALTO / 6;
    uint16_t  TITULO_X = config.ANCHO/2 - 200;

    DibujarTexto("TETRIZ KAMIKAZE",TITULO_X,TITULO_Y,4);
    gbt_volcar_backbuffer();

}
