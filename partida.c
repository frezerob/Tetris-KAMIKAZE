#include <stdio.h>
#include "partida.h"

int PartidaGuardar(EstadoPartida* estado)
{
    FILE* f = fopen(PARTIDA_FILE, "wb");
    if(!f) return -1;
    fwrite(estado, sizeof(EstadoPartida), 1, f);
    fclose(f);
    return 0;
}

int PartidaCargar(EstadoPartida* estado)
{
    FILE* f = fopen(PARTIDA_FILE, "rb");
    if(!f) return -1;
    fread(estado, sizeof(EstadoPartida), 1, f);
    fclose(f);
    return 0;
}

int PartidaExiste()
{
    FILE* f = fopen(PARTIDA_FILE, "rb");
    if(!f) return 0;
    fclose(f);
    return 1;
}

void PartidaBorrar()
{
    remove(PARTIDA_FILE);
}
