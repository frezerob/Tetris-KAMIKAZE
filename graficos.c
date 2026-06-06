#include <stdio.h>
#include <stdlib.h>
#include "graficos.h"
#include "GBT/gbt.h"
#include "funciones.h"
#include "core.h"
#include "config.h"

/**
 * Imprime en pantalla el tablero de juego.
 * Aplica un offset inicial a la posición del tablero
 * Distingue celdas vacías de celdas con bloques para una textura u otra
 */
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
/**
 * Dibuja la pieza en el tablero
 * Toma como argumento la pieza actual
 * Si la el elemento de la pieza es TR (transparente) o si su posición en Y es menor a 0 no se dibuja
 * Si el modo de juego es "CLASICO" la pieza no podrá dibujarse fuera de los bordes establecidos del tablero
 * Si el modo de juego es "DELUXE" la pieza podrá salirse de los bordes laterales para ser dibujado como si el tablero fuera redondo
 */
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
                    col += config.COL_TABLERO;
                }
                else if (col >= config.COL_TABLERO) {
                    col-=config.COL_TABLERO;
            }
                DibujarCelda(config.OFFSET_X + col * config.TAM_CELDA,
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
/**
 *
 * Matriz de 3 dimensiones usada para manejar las distintas texturas aplicadas en el juego
 * Las texturas disponibles son: BLOQUE, PLANO, BORDE, FONDO
 *
 */
const uint8_t texturas[CANT_TEXTURAS][8][8] =
{
    //BLOQUE
    {
        {2,2,2,2,2,2,2,2},
        {2,0,0,0,0,0,0,1},
        {2,0,0,0,0,0,0,1},
        {2,0,0,0,0,0,0,1},
        {2,0,0,0,0,0,0,1},
        {2,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1}
    },

    //PLANO
    {
        {0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}
    },

    //BORDE
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

/**
 * Dibujar el fondo del juego
 * ANCHO_CELDAS Y ALTO_CELDAS son la cantidad respectiva de celdas que hay disponibles (puede variar en base a la resolución)
 */
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
/**
 *
 * Dibujar celda toma una coordenada (X,Y), un color, un tamaño de celda y una textura para dibujar la celda en pantalla
 * Para saber que debe dibujar matea el pixel a la textura
 *
 */
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

/**
 * Función empleada para saber a que función dibujar el dibujo del bitmap.
 * Usa la fuente puesta en la cofiguración para saberlo
 */
void DibujarBitMap(const uint8_t bitmap[], uint16_t X, uint16_t Y, uint8_t color){
    if(config.FUENTE == fuente_8x8)
        DibujarBitMap8x8(bitmap,X,Y,color);
    else
        DibujarBitMap16x8(bitmap,X,Y,color);
}

/**
 * Dibujar el bitmap de 8 filas.
 * Realiza una comparacion "AND" entre el byte y un 1 desplazado 7-C digitos. Si devuelve cualquier cosa distinta de 0 dibuja
 * Se escala con la escala en configuración.
**/
void DibujarBitMap8x8(const uint8_t bitmap[8], uint16_t X, uint16_t Y, uint8_t color){

    uint8_t bits;
    for(int8_t f=0; f<TAM_FUENTE8X8; f++){

        bits =  bitmap[f];


        for(int8_t c=0; c<TAM_FUENTE8X8; c++){
            if(bits & (1 << (7 - c)))
            {
                uint16_t x_pixel = X + c * config.ESCALA_FUENTE;
                uint16_t y_pixel = Y + f * config.ESCALA_FUENTE;

                for(uint16_t sy = 0; sy < config.ESCALA_FUENTE; sy++) {
                    for(uint16_t sx = 0; sx < config.ESCALA_FUENTE; sx++) {
                        gbt_dibujar_pixel(x_pixel + sx, y_pixel + sy, color);
                    }
                }
            }
        }
    }
}
/**
 * Dibujar el bitmap de 16 filas.
 * Realiza una comparacion "AND" entre el byte y un 1 desplazado 7-C digitos. Si devuelve cualquier cosa distinta de 0 dibuja
 * La fuente se escala con config.ESCALA_FUENTE
**/
void DibujarBitMap16x8(const uint8_t bitmap[16], uint16_t X, uint16_t Y, uint8_t color)
{
    uint8_t bits;
    for(int8_t f=0; f<TAM_FUENTE16x8; f++){

        bits =  bitmap[f];


        for(int8_t c=0; c<TAM_FUENTE8X8; c++){
            if(bits & (1 << (7 - c)))
            {
                uint16_t x_pixel = X + c * config.ESCALA_FUENTE;
                uint16_t y_pixel = Y + f * config.ESCALA_FUENTE;

                for(uint16_t sy = 0; sy < config.ESCALA_FUENTE; sy++) {
                    for(uint16_t sx = 0; sx < config.ESCALA_FUENTE; sx++) {
                        gbt_dibujar_pixel(x_pixel + sx, y_pixel + sy, color);
                    }
                }
            }
        }
    }
}


/**
 *
 * Toma como argumento un caracter, se fija si es válido y lo deriva a la función DibujarBitMap para imprimirlo en pantalla
 *
 */
void DibujarLetra(char c, uint16_t X, uint16_t Y, uint8_t color)
{
    if(c < 'A' || c > 'Z') return;
      uint8_t indice = c - 'A';

    if(config.FUENTE == fuente_8x8) {
        DibujarBitMap(fuente8x8[indice], X, Y, color);
    } else {
        DibujarBitMap(fuente16x8[indice], X, Y, color);
    }
}
/**
 *
 * Toma como argumento un caracter, se fija si es válido y lo deriva a la función DibujarBitMap para imprimirlo en pantalla
 *
 */
void DibujarNumero(char num, uint16_t X, uint16_t Y, uint8_t color)
{
    if (num < '0' || num > '9') return;
    uint8_t indice = num - '0';

    if(config.FUENTE == fuente_8x8) {
        DibujarBitMap(fuente8x8_num[indice], X, Y, color);
    } else {
        DibujarBitMap(fuente16x8_num[indice], X, Y, color);
    }
}
/**
 *
 * Toma como argumento un caracter, se fija si es válido y lo deriva a la función correcta
 *
 */
void DibujarCaracter(char c, uint16_t X, uint16_t Y, uint8_t color)
{
    if(c >= 'A' && c <= 'Z')
        DibujarLetra(c, X, Y, color);
    else if(c >= '0' && c <= '9')
        DibujarNumero(c, X, Y, color);
    return;
}

/**
 *
 * Toma como argumento una cadena de texto y una posición (x,y) para dibujarlo con el color (también argumento)
 *
 */
void DibujarTexto(char* texto, uint16_t X, uint16_t Y, uint8_t color)
{
    while(*texto)
    {

        DibujarCaracter(*texto,X,Y,color);

        X += TAM_FUENTE8X8 * config.ESCALA_FUENTE + 1;

        texto++;

    }
}

/**
 * Toma como argumentos: Cadena de texto a imprimir, posición en eje Y, color y offset horizontal
 * Calcula la posición en X para que quede centrado con una formula
 */
void DibujarTextoCentrado(char* texto, uint16_t Y, uint8_t color, uint16_t OFFSET_X)
{
    uint16_t X = (config.ANCHO - CalcularAnchoTexto(texto))/2 + OFFSET_X;
    DibujarTexto(texto,X,Y,color);
}

/**
 * Toma como argumentos: Opción actual, arreglo de punteros a cadenas de texto y la cantidad de opciones
 * Imprime de manera centrada las distintas opciones del menú
 * Adicionalmente destaca la opción actual
 */
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
/**
 * Toma como argumentos: El puntaje, posición(X,Y) y un color
 * Dibujar el puntaje tomando la posición X;Y como base para empezar a escribir el número
 */
void DibujarPuntaje(int puntaje, uint16_t X, uint16_t Y, uint8_t color)
{
    char Spuntaje[17];
    itoa(puntaje,Spuntaje,10);
    DibujarTextoConSombra(Spuntaje,X,Y,color,T);

}
/**
 * Toma como argumentos una de las piezas futuras y un par X,Y
 * Dibujar la próxima pieza en el X;Y dados.
 */
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

/**
 * Toma como argumentos un par X;Y, ancho en celdas, alto en celdas, color y la textura
 * Dibuja el rectangulo alineado con las celdas en la posición X,Y
 */
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

/**
 * Toma como argumentos la pieza actual, la matriz de juego, el puntaje, las piezas próximas y el nombre del juegador
 * Renderiza el juego tomando los argumentos (fondo, piezas, estadísticas)
 */
void RenderizarJuego(PiezaActiva *p, matrix *m, int puntaje, int proximas[],char* nombre)
{
    gbt_borrar_backbuffer(N);
    DibujarFondo();

    DibujarTablero(m,0,0);
    DibujarPieza(p);
    DibujarRectangulo(config.OFFSET_X + (config.COL_TABLERO+1) * config.TAM_CELDA ,config.OFFSET_Y,15,20,N,PLANO);
    DibujarPuntaje(puntaje, config.OFFSET_X + (config.COL_TABLERO + 4) * config.TAM_CELDA + CalcularAnchoTexto("PUNTAJE"), config.OFFSET_Y + config.TAM_CELDA/2, W);
    DibujarTextoCentradoConSombra("PUNTAJE",config.OFFSET_Y + config.TAM_CELDA/2,W,-config.OFFSET_X -5,T);
    DibujarRectangulo(config.OFFSET_X,config.TAM_CELDA,26,2,N,PLANO);
    DibujarTextoConSombra("TETRIS KAMIKAZE",config.OFFSET_X,config.TAM_CELDA * 1.5, S,T);
    DibujarTextoConSombra(nombre,config.OFFSET_X*10,config.TAM_CELDA * 1.5,S,T);



    int xProx = config.OFFSET_X + (config.COL_TABLERO + 2) * config.TAM_CELDA;
    int yProx = config.OFFSET_Y + config.TAM_CELDA * 3;
    for(uint8_t i = 0; i < CANT_PROXIMAS; i++)
        DibujarProximaPieza(FORMAS[proximas[i]], xProx, yProx + i * (ORDEN * config.TAM_CELDA + 5));

    gbt_volcar_backbuffer();
}

/**
 * Toma como argumentos un texto, el centro X,Y, el color del texto, el color del fondo y la textura
 * Dibuja el texto en un recuadro
 */
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

/**
 * Toma como argumentos el texto, un X,Y, un color y el color de la sombra
 * Diuja el texto con un sombreado detras para mejor legibilidad del mismo
 */
void DibujarTextoConSombra(char* texto, uint16_t X, uint16_t Y, uint8_t color, uint8_t colorSombra)
{
    uint16_t offsetSombra = config.ESCALA_FUENTE;

    DibujarTexto(texto, X + offsetSombra, Y + offsetSombra, colorSombra);

    DibujarTexto(texto, X, Y, color);
}
/**
 * Toma como argumentos el texto, un "Y", un color y el color de la sombra
 * Diuja el texto centrado con un sombreado detras para mejor legibilidad del mismo
 */
void DibujarTextoCentradoConSombra(char* texto, uint16_t Y, uint8_t color, uint16_t OFFSET_X, uint8_t colorSombra)
{
    uint16_t X = (config.ANCHO - CalcularAnchoTexto(texto))/2 + OFFSET_X;
    DibujarTextoConSombra(texto,X,Y,color, colorSombra);
}

/**
 * Toma como argumento un indice de paleta
 * Aplica una paleta en base a un indice
 */
void AplicarPaleta(int indicePaleta)
{
    tGBT_ColorRGB paletas[3][PALETA_MAX_COLORES] = {

        {
            {0x00, 0x00, 0x00}, // N
            {0x00, 0xFF, 0xFF}, // I
            {0x00, 0xAA, 0xAA}, // I_OSCURO
            {0x99, 0xFF, 0xFF}, // I_CLARO
            {0xFF, 0xFF, 0x00}, // O
            {0xAA, 0xAA, 0x00}, // O_OSCURO
            {0xFF, 0xFF, 0x99}, // O_CLARO
            {0x80, 0x00, 0x80}, // T
            {0x55, 0x00, 0x55}, // T_OSCURO
            {0xCC, 0x99, 0xCC}, // T_CLARO
            {0x00, 0xFF, 0x00}, // S
            {0x00, 0xAA, 0x00}, // S_OSCURO
            {0x99, 0xFF, 0x99}, // S_CLARO
            {0xFF, 0x00, 0x00}, // Z
            {0xAA, 0x00, 0x00}, // Z_OSCURO
            {0xFF, 0x99, 0x99}, // Z_CLARO
            {0x00, 0x00, 0xFF}, // J
            {0x00, 0x00, 0xAA}, // J_OSCURO
            {0x99, 0x99, 0xFF}, // J_CLARO
            {0xFF, 0xA5, 0x00}, // L
            {0xAA, 0x6E, 0x00}, // L_OSCURO
            {0xFF, 0xD1, 0x99}, // L_CLARO
            {0x80, 0x80, 0x80}, // BRD
            {0x55, 0x55, 0x55}, // BRD_OSCURO
            {0xCC, 0xCC, 0xCC}, // BRD_CLARO
            {0xFF, 0xFF, 0xFF}, // W
            {0x01, 0x01, 0x01}, // TR
        },
        {
            {0x0F, 0x38, 0x0F}, // N
            {0x30, 0x62, 0x30}, // I
            {0x20, 0x52, 0x20}, // I_OSCURO
            {0x40, 0x72, 0x40}, // I_CLARO
            {0x8B, 0xAC, 0x0F}, // O
            {0x6B, 0x8C, 0x00}, // O_OSCURO
            {0xAB, 0xCC, 0x2F}, // O_CLARO
            {0x30, 0x62, 0x30}, // T
            {0x20, 0x52, 0x20}, // T_OSCURO
            {0x40, 0x72, 0x40}, // T_CLARO
            {0x8B, 0xAC, 0x0F}, // S
            {0x6B, 0x8C, 0x00}, // S_OSCURO
            {0xAB, 0xCC, 0x2F}, // S_CLARO
            {0x0F, 0x38, 0x0F}, // Z
            {0x00, 0x28, 0x00}, // Z_OSCURO
            {0x2F, 0x58, 0x2F}, // Z_CLARO
            {0x30, 0x62, 0x30}, // J
            {0x20, 0x52, 0x20}, // J_OSCURO
            {0x40, 0x72, 0x40}, // J_CLARO
            {0x8B, 0xAC, 0x0F}, // L
            {0x6B, 0x8C, 0x00}, // L_OSCURO
            {0xAB, 0xCC, 0x2F}, // L_CLARO
            {0x30, 0x62, 0x30}, // BRD
            {0x20, 0x52, 0x20}, // BRD_OSCURO
            {0x40, 0x72, 0x40}, // BRD_CLARO
            {0x9B, 0xBC, 0x0F}, // W
            {0x01, 0x01, 0x01}, // TR
        },

        {
            {0x00, 0x00, 0x00}, // N
            {0xDD, 0xDD, 0xDD}, // I
            {0xAA, 0xAA, 0xAA}, // I_OSCURO
            {0xFF, 0xFF, 0xFF}, // I_CLARO
            {0xBB, 0xBB, 0xBB}, // O
            {0x88, 0x88, 0x88}, // O_OSCURO
            {0xEE, 0xEE, 0xEE}, // O_CLARO
            {0x99, 0x99, 0x99}, // T
            {0x66, 0x66, 0x66}, // T_OSCURO
            {0xCC, 0xCC, 0xCC}, // T_CLARO
            {0xDD, 0xDD, 0xDD}, // S
            {0xAA, 0xAA, 0xAA}, // S_OSCURO
            {0xFF, 0xFF, 0xFF}, // S_CLARO
            {0x77, 0x77, 0x77}, // Z
            {0x44, 0x44, 0x44}, // Z_OSCURO
            {0xAA, 0xAA, 0xAA}, // Z_CLARO
            {0xBB, 0xBB, 0xBB}, // J
            {0x88, 0x88, 0x88}, // J_OSCURO
            {0xEE, 0xEE, 0xEE}, // J_CLARO
            {0x99, 0x99, 0x99}, // L
            {0x66, 0x66, 0x66}, // L_OSCURO
            {0xCC, 0xCC, 0xCC}, // L_CLARO
            {0x55, 0x55, 0x55}, // BRD
            {0x33, 0x33, 0x33}, // BRD_OSCURO
            {0x77, 0x77, 0x77}, // BRD_CLARO
            {0xFF, 0xFF, 0xFF}, // W
            {0x01, 0x01, 0x01}, // TR
        }
    };

    gbt_aplicar_paleta(paletas[indicePaleta], PALETA_MAX_COLORES, GBT_FORMATO_888);
}
