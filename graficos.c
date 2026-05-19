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
                if(m->mat[i][j]!=N)
                    DibujarCelda(PosX+j*config.TAM_CELDA,PosY+i*config.TAM_CELDA,m->mat[i][j],config.TAM_CELDA,BLOQUE);
                else
                    DibujarCelda(PosX+j*config.TAM_CELDA,PosY+i*config.TAM_CELDA,m->mat[i][j],config.TAM_CELDA,PLANO);
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
                                  config.TAM_CELDA,
                                  BLOQUE
                                  );
        }
    }
}


const uint8_t texturas[CANT_TEXTURAS][8][8] =
{
    {
        {W,W,W,W,W,W,W,W},
        {1,W,W,W,W,W,W,1},
        {1,1,0,0,0,0,1,1},
        {1,1,0,0,0,0,1,1},
        {1,1,0,0,0,0,1,1},
        {1,1,0,0,0,0,1,1},
        {1,BRD,BRD,BRD,BRD,BRD,BRD,1},
        {BRD,BRD,BRD,BRD,BRD,BRD,BRD,BRD}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    }

};

void DibujarCelda(uint16_t X, uint16_t Y, uint8_t color, uint8_t TAMANIO, eTexturas textura)
{
    uint16_t LIM_Y = Y + TAMANIO, LIM_X = X + TAMANIO;
    for(uint16_t f = Y; f<LIM_Y; f++)
        for(uint16_t c = X; c< LIM_X; c++){
            uint8_t tex_f = ((f - Y) * 8) / TAMANIO;
            uint8_t tex_c = ((c - X) * 8) / TAMANIO;

            uint8_t tipo_pixel = texturas[textura][tex_f][tex_c];
            if (textura == PLANO)
                gbt_dibujar_pixel(c,f,color);
            else
                switch(tipo_pixel)
                {
                    case 0:
                        gbt_dibujar_pixel(c,f,color);
                        break;
                    case 1:
                        gbt_dibujar_pixel(c,f,color+1);
                        break;
                    case W:
                        gbt_dibujar_pixel(c,f,W);
                        break;
                    case BRD:
                        gbt_dibujar_pixel(c,f,BRD);

                }
        }
}


void DibujarBitMap8x8(const uint8_t bitmap[8], uint16_t X, uint16_t Y, uint8_t color){

    uint8_t bits;
    for(int8_t f=0; f<TAM_FUENTE8X8; f++){

        bits =  bitmap[f];


        for(int8_t c=0; c<TAM_FUENTE8X8; c++){
            if(bits & (1 << (7 - c)))
            {
                uint16_t x_pixel = X + c * config.ESCALA_FUENTE;
                uint16_t y_pixel = Y + f * config.ESCALA_FUENTE;

                // Dibujamos un bloque sólido del tamaño de config.ESCALA_FUENTE|
                for(uint16_t sy = 0; sy < config.ESCALA_FUENTE; sy++) {
                    for(uint16_t sx = 0; sx < config.ESCALA_FUENTE; sx++) {
                        gbt_dibujar_pixel(x_pixel + sx, y_pixel + sy, color);
                    }
                }
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


void DibujarTextoCentrado(char* texto, uint16_t Y, uint8_t color)
{
    uint16_t X = (config.ANCHO - CalcularAnchoTexto(texto))/2;
    DibujarTexto(texto,X,Y,color);
}

void ImprimirMenu(uint8_t opcion, char* opciones_menu[], size_t cant_opciones)
{
    gbt_borrar_backbuffer(BRD);
    uint16_t Y=0;
    for(uint8_t op = 0; op<cant_opciones; op++){
        Y += config.ALTO/(cant_opciones*2);
        if(op == opcion)
            DibujarTextoCentrado(opciones_menu[op],Y,W);
        else
            DibujarTextoCentrado(opciones_menu[op],Y,4);
    }
    gbt_volcar_backbuffer();

}

void DibujarPuntaje(int puntaje, uint16_t X, uint16_t Y, uint8_t color)
{
    char Spuntaje[17];
    itoa(puntaje,Spuntaje,10);
    DibujarTexto(Spuntaje,X,Y,color);

}

void DibujarProximaPieza(uint8_t (*forma)[16], int X, int Y)
{
    for(int i = 0; i < ORDEN; i++){
        for(int j = 0; j < ORDEN; j++){
            if(forma[0][i*ORDEN+j] != TR)
                DibujarCelda(X + j * config.TAM_CELDA,
                             Y + i * config.TAM_CELDA,
                             forma[0][i*ORDEN+j],
                             config.TAM_CELDA);
        }
    }
}
