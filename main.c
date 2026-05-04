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

    if(MenuIniciar() != 0)
        return INIT_ERR;
    if(MatrizIniciar(&m, FIL_TABLERO, COL_TABLERO) == INIT_ERR)
        return INIT_ERR;

    tipoPieza(&p, generarPiezaAleatoria());

    tGBT_Temporizador* temporizador = gbt_temporizador_crear(0.2);
    if(!temporizador){
        printf("%s", gbt_obtener_log());
        return TEMPO_ERR;
    }

    uint8_t X_origen = 0;
    uint8_t Y_origen = 0;
    uint8_t corriendo = 1;
    uint8_t fijada = 0;
    eGBT_Tecla tecla;

    while(corriendo)
    {
        fijada = 0;
        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_ESCAPE)
            corriendo = 0;

        // GRAVEDAD POR TIEMPO — va primero
        if(gbt_temporizador_consumir(temporizador)){
            PiezaMoverAbajo(&p);
            if(PiezaDetectarColision(&p, &m)){
                PiezaMoverArriba(&p);
                PiezaVolcar(&m, &p);
                int fila;
                while((fila = MatrizFilCompleta(&m)) != -1)
                    MatrizEliminarFila(&m, fila);
                tipoPieza(&p, generarPiezaAleatoria());
                fijada = 1;
            }
        }

        // TECLAS — solo si la pieza no se fijó este frame
        if(!fijada){
            if(tecla == GBTK_a){
                PiezaMoverIzq(&p);
                if(PiezaDetectarColision(&p, &m))
                    PiezaMoverDer(&p);
            }
            if(tecla == GBTK_d){
                PiezaMoverDer(&p);
                if(PiezaDetectarColision(&p, &m))
                    PiezaMoverIzq(&p);
            }
            if(tecla == GBTK_w){
                PiezaRotarDerecha(&p);
                if(PiezaDetectarColision(&p, &m))
                    PiezaRotarIzquierda(&p);
            }
            if(tecla == GBTK_s){
                PiezaMoverAbajo(&p);
                if(PiezaDetectarColision(&p, &m)){
                    PiezaMoverArriba(&p);
                    PiezaVolcar(&m, &p);
                    int fila;
                    while((fila = MatrizFilCompleta(&m)) != -1)
                        MatrizEliminarFila(&m, fila);
                    tipoPieza(&p, generarPiezaAleatoria());
                }
            }
        }

        // DIBUJADO
        gbt_borrar_backbuffer(BRD);
        DibujarTablero(&m, X_origen, Y_origen);
        DibujarPieza(&p);
        gbt_volcar_backbuffer();
    }

    gbt_temporizador_destruir(temporizador);
    gbt_destruir_ventana();

    return 0;
}
