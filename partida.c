#include <stdio.h>
#include "partida.h"


/**
 * Toma como argumento un puntero al estado de partida
 * Crea un archivo PARTIDA_FILE para ESCRIBIR el estado de la partida en el
 * Devuelve -1 si no puede abrirlo y 0 si fue exitoso
 */
int PartidaGuardar(EstadoPartida* estado)
{
    FILE* f = fopen(PARTIDA_FILE, "wb");
    if(!f) return -1;
    fwrite(estado, sizeof(EstadoPartida), 1, f);
    fclose(f);
    return 0;
}
/**
 * Toma como argumento un puntero a un estado de partida
 * Intenta abrir un archivo PARTIDA_FILE para cargar el estado de la partida en el puntero a estado de partida
 * Devuelve -1 si no puede abrirlo
 * Devuelve 0 si fue exitoso
 */
int PartidaCargar(EstadoPartida* estado)
{
    FILE* f = fopen(PARTIDA_FILE, "rb");
    if(!f) return -1;
    fread(estado, sizeof(EstadoPartida), 1, f);
    fclose(f);
    return 0;
}

/**
 * Función dedicada a verificar si existe un estado de aprtida guardado
 * Devuelve 0 si no lo encuentra
 * Devuelve 1 si lo encuentra
 */
int PartidaExiste()
{
    FILE* f = fopen(PARTIDA_FILE, "rb");
    if(!f) return 0;
    fclose(f);
    return 1;
}

/**
 * Borra el estado de partida con nombre PARTIDA_FILE
 */
void PartidaBorrar()
{
    remove(PARTIDA_FILE);
}
