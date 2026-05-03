#include "funciones.h"
#include "core.h"

void semilla()
{
    srand(time(NULL));
}
/*
* Genera una pieza al azar
*/
int generarPiezaAleatoria() {
    return (rand() % 7);
}

int PIEZA_I[4][16] = {
    {TR,TR,TR,TR, I,I,I,I, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,TR,I,TR, TR,TR,I,TR, TR,TR,I,TR, TR,TR,I,TR},
    {TR,TR,TR,TR, TR,TR,TR,TR, I,I,I,I, TR,TR,TR,TR},
    {TR,I,TR,TR, TR,I,TR,TR, TR,I,TR,TR, TR,I,TR,TR}
};

int PIEZA_J[4][16] = {
    {J,TR,TR,TR, J,J,J,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,J,J,TR, TR,J,TR,TR, TR,J,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, J,J,J,TR, TR,TR,J,TR, TR,TR,TR,TR},
    {TR,J,TR,TR, TR,J,TR,TR, J,J,TR,TR, TR,TR,TR,TR}
};

int PIEZA_L[4][16] = {
    {TR,TR,L,TR, L,L,L,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,L,TR,TR, TR,L,TR,TR, TR,L,L,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, L,L,L,TR, L,TR,TR,TR, TR,TR,TR,TR},
    {L,L,TR,TR, TR,L,TR,TR, TR,L,TR,TR, TR,TR,TR,TR}
};

int PIEZA_O[4][16] = {
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR}
};

int PIEZA_S[4][16] = {
    {TR,S,S,TR, S,S,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,S,TR,TR, TR,S,S,TR, TR,TR,S,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, TR,S,S,TR, S,S,TR,TR, TR,TR,TR,TR},
    {S,TR,TR,TR, S,S,TR,TR, TR,S,TR,TR, TR,TR,TR,TR}
};

int PIEZA_T[4][16] = {
    {TR,T,TR,TR, T,T,T,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,T,TR,TR, TR,T,T,TR, TR,T,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, T,T,T,TR, TR,T,TR,TR, TR,TR,TR,TR},
    {TR,T,TR,TR, T,T,TR,TR, TR,T,TR,TR, TR,TR,TR,TR}
};

int PIEZA_Z[4][16] = {
    {Z,Z,TR,TR, TR,Z,Z,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,TR,Z,TR, TR,Z,Z,TR, TR,Z,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, Z,Z,TR,TR, TR,Z,Z,TR, TR,TR,TR,TR},
    {TR,Z,TR,TR, Z,Z,TR,TR, Z,TR,TR,TR, TR,TR,TR,TR}
};



int (*FORMAS[7])[16] = {
    PIEZA_I, PIEZA_J, PIEZA_L, PIEZA_O, PIEZA_S, PIEZA_T, PIEZA_Z
};


void tipoPieza(PiezaActiva* pieza, int tipoSeleccionado) {

    pieza->forma = FORMAS[tipoSeleccionado];
    // Valores iniciales
    pieza->rotacion = 0;
    pieza->posX = 5;

    pieza->posY = 0;
}
/*
int8_t PiezaPuedeRotar(PiezaActiva* p)
{
    uint8_t flag = 0;
    for(uint8_t i = 0; i<ORDEN; i++){
        for(uint8_t j = 0; j<ORDEN; j++){
            if((p->forma[p->rotacion][i * 4 + j) != 0 && )
        }
    }
}

*/
void PiezaMoverArriba(PiezaActiva* p)
{
    p->posY--;
}

void PiezaMoverAbajo(PiezaActiva* p)
{
    p->posY++;
}

void PiezaMoverIzq(PiezaActiva* p)
{
    if(p->posX>1)
        p->posX--;
}
void PiezaMoverDer(PiezaActiva* p)
{
    if(p->posX<10)
        p->posX++;
}

int8_t PiezaDetectarColision(PiezaActiva* p)
{
    for(uint8_t i=0; i<ORDEN;i++){
        for(uint8_t j=0; j<ORDEN; j++){
            if (p->forma[p->rotacion][i*4+j]!=TR && gbt_obtener_color_pixel(p->posX + j,p->posY + i) != N){
                printf("1");
                return 1;
            }

        }
    }
    return 0;
}

