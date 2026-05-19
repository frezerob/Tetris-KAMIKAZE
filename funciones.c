#include "funciones.h"
#include "core.h"

uint8_t PiezaAnterior;

void semilla()
{
    srand(time(NULL));
}
/*
* Genera una pieza al azar
*/
uint8_t generarPiezaAleatoria() {
    uint8_t Aux;
    do{
        Aux = (rand() % 7);
    }
    while(Aux == PiezaAnterior);
    PiezaAnterior = Aux;
    return PiezaAnterior;
}



uint8_t PIEZA_I[4][16] = {
    {TR,TR,TR,TR, I,I,I,I, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,TR,I,TR, TR,TR,I,TR, TR,TR,I,TR, TR,TR,I,TR},
    {TR,TR,TR,TR, TR,TR,TR,TR, I,I,I,I, TR,TR,TR,TR},
    {TR,I,TR,TR, TR,I,TR,TR, TR,I,TR,TR, TR,I,TR,TR}
};

uint8_t PIEZA_J[4][16] = {
    {J,TR,TR,TR, J,J,J,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,J,J,TR, TR,J,TR,TR, TR,J,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, J,J,J,TR, TR,TR,J,TR, TR,TR,TR,TR},
    {TR,J,TR,TR, TR,J,TR,TR, J,J,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_L[4][16] = {
    {TR,TR,L,TR, L,L,L,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,L,TR,TR, TR,L,TR,TR, TR,L,L,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, L,L,L,TR, L,TR,TR,TR, TR,TR,TR,TR},
    {L,L,TR,TR, TR,L,TR,TR, TR,L,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_O[4][16] = {
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_S[4][16] = {
    {TR,S,S,TR, S,S,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,S,TR,TR, TR,S,S,TR, TR,TR,S,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, TR,S,S,TR, S,S,TR,TR, TR,TR,TR,TR},
    {S,TR,TR,TR, S,S,TR,TR, TR,S,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_T[4][16] = {
    {TR,T,TR,TR, T,T,T,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,T,TR,TR, TR,T,T,TR, TR,T,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, T,T,T,TR, TR,T,TR,TR, TR,TR,TR,TR},
    {TR,T,TR,TR, T,T,TR,TR, TR,T,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_Z[4][16] = {
    {Z,Z,TR,TR, TR,Z,Z,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,TR,Z,TR, TR,Z,Z,TR, TR,Z,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, Z,Z,TR,TR, TR,Z,Z,TR, TR,TR,TR,TR},
    {TR,Z,TR,TR, Z,Z,TR,TR, Z,TR,TR,TR, TR,TR,TR,TR}
};



uint8_t (*FORMAS[7])[16] = {
    PIEZA_I, PIEZA_J, PIEZA_L, PIEZA_O, PIEZA_S, PIEZA_T, PIEZA_Z
};


void tipoPieza(PiezaActiva* pieza, uint8_t tipoSeleccionado) {

    pieza->forma = FORMAS[tipoSeleccionado];
    // Valores iniciales
    pieza->rotacion = 0;
    pieza->posX = COL_TABLERO/2;

    pieza->posY = -1;
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
    p->posX--;
}
void PiezaMoverDer(PiezaActiva* p)
{
    p->posX++;
}

void PiezaRotarDerecha(PiezaActiva* p)
{
    p->rotacion = (p->rotacion + 1) % 4;
}


// funciones.c

int8_t PiezaDetectarColision(PiezaActiva* p, matrix* m)
{
    for(uint8_t i = 0; i < ORDEN; i++){
        for(uint8_t j = 0; j < ORDEN; j++){
            if(p->forma[p->rotacion][i*4+j] == TR) continue;

            int fila = p->posY + i;
            int col  = p->posX + j;

            if(fila >= FIL_TABLERO) return 1;          // piso
            if(col < 0 || col >= COL_TABLERO) return 1; // bordes
            if(fila >= 0 && m->mat[fila][col] != N) return 1; // celda ocupada
        }
    }
    return 0;
}

void PiezaRotarIzquierda(PiezaActiva* p)
{
    p->rotacion = (p->rotacion + 3) % 4;
}

int RecalcularVelocidad(int velInicial, int piezas)
{
    int niveles = piezas / 10;
    int vel = velInicial;
    for(int i = 0; i < niveles; i++)
        vel = (vel * 97) / 100;
    return vel;
}

int VelocidadSegunDificultad(int dificultad)
{
    if(dificultad == 0) return 1000;  // FACIL
    if(dificultad == 1) return 1500;  // NORMAL
    if(dificultad == 2) return 200;   // DIFICIL
    return 1000;
}
