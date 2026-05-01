/*
Apellido: Freccero, Alejo Benjamin
DNI: 47263450
Usuario: frezerob
Entrega: No
Apellido: Quevedo, Lautaro Joaquin
DNI: 47024422
Usuario: LautaroJQ
Entrega: No
Apellido: Conti, Dante Javier
DNI: 44510761
Usuario: dantejaconti
Entrega: No
*/
#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"
#include "menu.h"
#include "tdatablero.h"
#include "funciones.h"




tGBT_ColorRGB paleta[] = {
    {0x00,0x00,0x00}, // 0 -> NEGRO
    {0xFF,0x00,0x00}, // 1 -> ROJO
    {0x00,0xFF,0x00}, // 2 -> VERDE
    {0x00,0x00,0xFF}, // 3 -> AZUL
    {0xFF,0xFF,0xFF}, // 4 -> BLANCO
    {0x01,0x01,0x01}  // 5 -> TRANSPARENTE

};


int main(int argc, char* argv[])
{
    semilla();
    Stablero tablero;
    if(MenuIniciar()!=0)
        return INIT_ERROR;
    if(StableroIniciar(&tablero)==INIT_ERROR)
        return INIT_ERROR;
    tablero.tab[0][1]=24;
    tablero.tab[4][5]= 1;

    StableroMostrar(&tablero);
    printf("\n");
    printf("\n");
    StableroDesplazarFil(&tablero,4);
    printf("\n");
    printf("\n");
    StableroMostrar(&tablero);


    uint8_t corriendo = 1;
    eGBT_Tecla tecla; // Estructura tecla que detecta las teclas presionadas
    while(corriendo)
    {
        gbt_procesar_entrada(); // Actualiza el estado de las teclas
        tecla = gbt_obtener_tecla_presionada(); // Carga en la estructura tecla la tecla presionada
        if(tecla == GBTK_ESCAPE)
            corriendo = 0;


    }


    return 0;
}
