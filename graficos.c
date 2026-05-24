#include <stdio.h>
#include <stdlib.h>
#include "graficos.h"
#include "GBT/gbt.h"
#include "funciones.h"
#include "core.h"
#include "config.h"
void DibujarTablero(matrix* m, uint16_t X, uint16_t Y){

    uint16_t PosX = X + config.OFFSET_X;
    uint16_t PosY = Y + config.OFFSET_Y;
    for(uint8_t i=0; i<m->fil;i++){
        for(uint8_t j=0; j<m->col; j++){
                if(m->mat[i][j]!=N)
                    DibujarCelda(PosX+j*config.TAM_CELDA,PosY+i*config.TAM_CELDA,m->mat[i][j],config.TAM_CELDA,BLOQUE);
                else
                    DibujarCelda(PosX+j*config.TAM_CELDA,PosY+i*config.TAM_CELDA,N,config.TAM_CELDA,FONDO);
        }
    }
}

void DibujarPieza(PiezaActiva* p)
{
    for(uint8_t i = 0; i < ORDEN; i++){
        for(uint8_t j = 0; j < ORDEN; j++){
            if(p->forma[p->rotacion][i*ORDEN+j] == TR)
                continue;
            if((p->posY+i)<0)
                continue;
            int16_t fil = p->posY + i;
            int16_t col = p->posX + j;

            if(config.MODO == DELUXE){

                if (col < 0) {
                    // Si se fue por la izquierda (ej: -1), aparece a la derecha (9)
                    col += config.COL_TABLERO;
                }
                else if (col >= config.COL_TABLERO) {
                    // Si se fue por la derecha (ej: 10), aparece a la izquierda (0)
                    col-=config.COL_TABLERO;
            }
                DibujarCelda(config.OFFSET_X + col * config.TAM_CELDA,  // offset solo acá
                  config.OFFSET_Y + fil  * config.TAM_CELDA,
                  p->forma[p->rotacion][i * ORDEN + j],
                  config.TAM_CELDA,
                  BLOQUE
                );
            }
            else{
                if(col >= 0 && col < config.COL_TABLERO) {
                    DibujarCelda(
                        config.OFFSET_X + col * config.TAM_CELDA,
                        config.OFFSET_Y + fil * config.TAM_CELDA,
                        p->forma[p->rotacion][i * ORDEN + j],
                        config.TAM_CELDA,
                        BLOQUE
                    );
                }
            }
        }
    }
}

const uint8_t texturas[CANT_TEXTURAS][8][8] =
{
    //BLOQUE
    {
        {2,2,2,2,2,2,2,2}, // Fila superior: Brillo Claro (color + 2)
        {2,0,0,0,0,0,0,1}, // Izquierda: Brillo (color + 2) | Derecha: Sombra (color + 1)
        {2,0,0,0,0,0,0,1}, // Centro: Cuerpo normal de la pieza (color)
        {2,0,0,0,0,0,0,1},
        {2,0,0,0,0,0,0,1},
        {2,0,0,0,0,0,0,1},
        {2,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1}  // Fila inferior: Sombra Oscura (color + 1)
    },

    //PLANO
    {
        {0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}
    },

    //BORDE -> Textura de ladrillo estático para las paredes del tablero
    {
        {BRD_CLARO, BRD_CLARO, BRD_CLARO, BRD_CLARO, BRD_CLARO, BRD_CLARO, BRD_CLARO, BRD_CLARO},
        {BRD_CLARO, BRD,       BRD,       BRD,       BRD,       BRD,       BRD,       BRD_OSCURO},
        {BRD_CLARO, BRD,       BRD,       BRD,       BRD,       BRD,       BRD,       BRD_OSCURO},
        {BRD_CLARO, BRD,       BRD,       BRD,       BRD,       BRD,       BRD,       BRD_OSCURO},
        {BRD_CLARO, BRD,       BRD,       BRD,       BRD,       BRD,       BRD,       BRD_OSCURO},
        {BRD_CLARO, BRD,       BRD,       BRD,       BRD,       BRD,       BRD,       BRD_OSCURO},
        {BRD_CLARO, BRD,       BRD,       BRD,       BRD,       BRD,       BRD,       BRD_OSCURO},
        {BRD_OSCURO,BRD_OSCURO,BRD_OSCURO,BRD_OSCURO,BRD_OSCURO,BRD_OSCURO,BRD_OSCURO,BRD_OSCURO}
    },
    //FONDO
    {
        {BRD_OSCURO, BRD_OSCURO, BRD_OSCURO, BRD_OSCURO, BRD_OSCURO, BRD_OSCURO, BRD_OSCURO, BRD_OSCURO},
        {BRD_OSCURO, 0, 0, 0, 0, 0, 0, 0},
        {BRD_OSCURO, 0, 0, 0, 0, 0, 0, 0},
        {BRD_OSCURO, 0, 0, 0, 0, 0, 0, 0},
        {BRD_OSCURO, 0, 0, 0, 0, 0, 0, 0},
        {BRD_OSCURO, 0, 0, 0, 0, 0, 0, 0},
        {BRD_OSCURO, 0, 0, 0, 0, 0, 0, 0},
        {BRD_OSCURO, 0, 0, 0, 0, 0, 0, 0}
    }
};

void DibujarFondo()
{
    const uint16_t ANCHO_CELDAS = config.ANCHO / config.TAM_CELDA;
    const uint16_t ALTO_CELDAS = config.ALTO / config.TAM_CELDA;
    for(uint8_t f = 0; f < ALTO_CELDAS; f++){
        for(uint16_t c = 0; c < ANCHO_CELDAS; c++){
            uint16_t X = (uint16_t)config.TAM_CELDA * c;
            uint16_t Y = (uint16_t)config.TAM_CELDA * f;
            DibujarCelda(X,Y,0,config.TAM_CELDA,BORDE);
        }
    }
}

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
                    case 2:
                        gbt_dibujar_pixel(c,f,color+2);
                        break;
                    default:
                        gbt_dibujar_pixel(c,f,tipo_pixel);

                }
        }
}


void DibujarBitMap(const uint8_t bitmap[], uint16_t X, uint16_t Y, uint8_t color){
    if(config.FUENTE == fuente_8x8)
        DibujarBitMap8x8(bitmap,X,Y,color);
    else
        DibujarBitMap8x16(bitmap,X,Y,color);
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

void DibujarBitMap8x16(const uint8_t bitmap[16], uint16_t X, uint16_t Y, uint8_t color)
{
    uint8_t bits;
    for(int8_t f=0; f<TAM_FUENTE16x8; f++){

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


//
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
    else if(c>='0' && c<='9')
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


void DibujarTextoCentrado(char* texto, uint16_t Y, uint8_t color, uint16_t OFFSET_X)
{
    uint16_t X = (config.ANCHO - CalcularAnchoTexto(texto))/2 + OFFSET_X;
    DibujarTexto(texto,X,Y,color);
}

void ImprimirMenu(uint8_t opcion, char* opciones_menu[], size_t cant_opciones)
{
    gbt_borrar_backbuffer(N);
    DibujarFondo();
    uint16_t Y=0;
    for(uint8_t op = 0; op<cant_opciones; op++){
        Y += config.ALTO/(cant_opciones*2);
        if(op == opcion)
            DibujarTextoCentradoConSombra(opciones_menu[op],Y,W,0,N);
        else
            DibujarTextoCentradoConSombra(opciones_menu[op],Y,O,0,N);
    }
    gbt_volcar_backbuffer();

}

void DibujarPuntaje(int puntaje, uint16_t X, uint16_t Y, uint8_t color)
{
    char Spuntaje[17];
    itoa(puntaje,Spuntaje,10);
    DibujarTextoConSombra(Spuntaje,X,Y,color,T);

}

void DibujarProximaPieza(uint8_t (*forma)[16], uint16_t X, uint16_t Y)
{
    for(uint8_t i = 0; i < ORDEN; i++){
        for(uint16_t j = 0; j < ORDEN; j++){
            if(forma[0][i*ORDEN+j] != TR)
                DibujarCelda(X + j * config.TAM_CELDA,
                             Y + i * config.TAM_CELDA,
                             forma[0][i*ORDEN+j],
                             config.TAM_CELDA,BLOQUE);
        }
    }
}

void DibujarRectangulo(uint16_t X, uint16_t Y, uint16_t ancho_celdas, uint16_t alto_celdas, uint8_t color, eTexturas textura)
{

    uint16_t X_ALINEADO = X/config.TAM_CELDA * config.TAM_CELDA;
    uint16_t Y_ALINEADO = Y/config.TAM_CELDA * config.TAM_CELDA;
    uint16_t INICIO_CELDA_X;
    uint16_t INICIO_CELDA_Y;
    for(uint16_t f = 0; f < alto_celdas; f++){
        for(uint16_t c = 0; c< ancho_celdas ; c++){
            INICIO_CELDA_X = X_ALINEADO + (config.TAM_CELDA * c);
            INICIO_CELDA_Y = Y_ALINEADO + (config.TAM_CELDA * f);
            DibujarCelda(INICIO_CELDA_X,INICIO_CELDA_Y,color,config.TAM_CELDA,textura);
        }
    }
}


void RenderizarJuego(PiezaActiva *p, matrix *m, int puntaje, int proximas[],char* nombre)
{
    gbt_borrar_backbuffer(N);
    DibujarFondo();

    DibujarTablero(m,0,0);
    DibujarPieza(p);
    //RECTANGULO DE ESTADISTICAS
    DibujarRectangulo(config.OFFSET_X + (config.COL_TABLERO+1) * config.TAM_CELDA ,config.OFFSET_Y,15,20,N,PLANO);
    DibujarPuntaje(puntaje, config.OFFSET_X + (config.COL_TABLERO + 4) * config.TAM_CELDA + CalcularAnchoTexto("PUNTAJE"), config.OFFSET_Y + config.TAM_CELDA/2, W);
    DibujarTextoCentradoConSombra("PUNTAJE",config.OFFSET_Y + config.TAM_CELDA/2,W,-config.OFFSET_X -5,T);
    //RECTANGULO TITULO
    DibujarRectangulo(config.OFFSET_X,config.TAM_CELDA,26,2,N,PLANO);
    DibujarTextoConSombra("TETRIS KAMIKAZE",config.OFFSET_X,config.TAM_CELDA * 1.5, S,T);
    DibujarTextoConSombra(nombre,config.OFFSET_X*10,config.TAM_CELDA * 1.5,S,T);



    int xProx = config.OFFSET_X + (config.COL_TABLERO + 2) * config.TAM_CELDA;
    int yProx = config.OFFSET_Y + config.TAM_CELDA * 3;
    for(uint8_t i = 0; i < CANT_PROXIMAS; i++)
        DibujarProximaPieza(FORMAS[proximas[i]], xProx, yProx + i * (ORDEN * config.TAM_CELDA + 5));

    gbt_volcar_backbuffer();
}


void DibujarTextoEnRecuadro(char* texto, uint16_t centroX, uint16_t centroY, uint8_t colorTexto, uint8_t colorFondo, eTexturas texturaBorde)
{
    uint16_t anchoTexto = CalcularAnchoTexto(texto);
    uint16_t altoTexto = TAM_FUENTE8X8 * config.ESCALA_FUENTE;


    uint16_t margenX = config.TAM_CELDA;
    uint16_t margenY = config.TAM_CELDA / 2;

    uint16_t anchoRecuadro = anchoTexto + (margenX * 2);
    uint16_t altoRecuadro = altoTexto + (margenY * 2);

    uint16_t celdasAncho = (anchoRecuadro / config.TAM_CELDA) + 1;
    uint16_t celdasAlto = (altoRecuadro / config.TAM_CELDA) + 1;

    uint16_t posX = centroX - (celdasAncho * config.TAM_CELDA) / 2;
    uint16_t posY = centroY - (celdasAlto * config.TAM_CELDA) / 2;

    DibujarRectangulo(posX, posY, celdasAncho, celdasAlto, colorFondo, texturaBorde);

    uint16_t textoX = centroX - (anchoTexto / 2);
    uint16_t textoY = centroY - (altoTexto / 2);

    DibujarTexto(texto, textoX, textoY, colorTexto);
}

void DibujarTextoConSombra(char* texto, uint16_t X, uint16_t Y, uint8_t color, uint8_t colorSombra)
{
    uint16_t offsetSombra = config.ESCALA_FUENTE;

    DibujarTexto(texto, X + offsetSombra, Y + offsetSombra, colorSombra);

    DibujarTexto(texto, X, Y, color);
}

void DibujarTextoCentradoConSombra(char* texto, uint16_t Y, uint8_t color, uint16_t OFFSET_X, uint8_t colorSombra)
{
    uint16_t X = (config.ANCHO - CalcularAnchoTexto(texto))/2 + OFFSET_X;
    DibujarTextoConSombra(texto,X,Y,color, colorSombra);
}
