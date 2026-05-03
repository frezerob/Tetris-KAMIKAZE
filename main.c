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
#include "graficos.h"





int main(int argc, char* argv[])
{
    semilla();
    matrix m;
    PiezaActiva p;

    if(MenuIniciar()!=0)
        return INIT_ERR;
    if(MatrizIniciar(&m,FIL_TABLERO,COL_TABLERO)==INIT_ERR)
        return INIT_ERR;

    tipoPieza(&p,generarPiezaAleatoria());

    //DibujarTablero(&m,0,0);
    //DibujarPieza(&p);


    //Inicia temporizador de 0 segundo
    tGBT_Temporizador* temporizador = gbt_temporizador_crear(1.0);
    if(!temporizador){
        printf("%s",gbt_obtener_log());
        return TEMPO_ERR;
    }

    uint8_t X_origen = 0;
    uint8_t Y_origen = 0;

    uint8_t corriendo = 1;
    eGBT_Tecla tecla; // Estructura tecla que detecta las teclas presionadas
    while(corriendo)
    {

        //Borramos el buffer
        gbt_borrar_backbuffer(BRD);
        DibujarTablero(&m,X_origen,Y_origen);


        //PROCESADO DE TECLAS
        gbt_procesar_entrada(); // Actualiza el estado de las teclas
        tecla = gbt_obtener_tecla_presionada(); // Carga en la estructura tecla la tecla presionada
        if(tecla == GBTK_ESCAPE)
            corriendo = 0;

        //MOVIMIENTOS Y ROTACIONES
        if((tecla == GBTK_ESPACIO) ){
            printf("1");
        }
        if(tecla == GBTK_a)
            PiezaMoverIzq(&p);
        if(tecla == GBTK_d)
            PiezaMoverDer(&p);
        if(tecla == GBTK_s){
            PiezaMoverAbajo(&p);
            if(PiezaDetectarColision(&p)){
                PiezaMoverArriba(&p);
                DibujarPieza(&p);
                PiezaVolcar(&m);
                tipoPieza(&p,generarPiezaAleatoria());
            }
        }


        //MOVIMIENTOS POR TIEMPO
        if(gbt_temporizador_consumir(temporizador)){
            PiezaMoverAbajo(&p);
            if(PiezaDetectarColision(&p)){
                PiezaMoverArriba(&p);
                DibujarPieza(&p);
                PiezaVolcar(&m);
                tipoPieza(&p,generarPiezaAleatoria());
            }
        }



        //DIBUJADO Y VOLCADO

        DibujarPieza(&p);
        gbt_volcar_backbuffer();

    }

    gbt_destruir_ventana();


    return 0;
}
