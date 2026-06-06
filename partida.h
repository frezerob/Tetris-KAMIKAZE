#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "core.h"
#include "tdatablero.h"

#define PARTIDA_FILE "partida.bin"
#define MAX_COL 16
#define MAX_FIL 20

typedef struct {
    uint8_t mat[MAX_FIL][MAX_COL];
    int fil;
    int col;
    int posX;
    int posY;
    int rotacion;
    int tipoPiezaActual;
    int proximas[CANT_PROXIMAS];
    int puntaje;
    int piezasCaidas;
    int velActual;
    char nombre[MAX_NOMBRE + 1];
    ModoJuego MODO;
} EstadoPartida;

int PartidaGuardar(EstadoPartida* estado);
int PartidaCargar(EstadoPartida* estado);
int PartidaExiste();
void PartidaBorrar();

#endif
