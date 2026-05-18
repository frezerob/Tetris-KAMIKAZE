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


int8_t IniciarSistema(int argc, char* argv[]);



int main(int argc, char* argv[])
{
    // Deteccion de argumentos (Ancho y Alto)

    IniciarSistema(argc,argv);


    semilla();
    matrix m;
    PiezaActiva p;

    if(MenuIniciar(config) != 0)
        return INIT_ERR;
    if(MatrizIniciar(&m, FIL_TABLERO, COL_TABLERO) == INIT_ERR)
        return INIT_ERR;
    tipoPieza(&p, generarPiezaAleatoria());

    tGBT_Temporizador* temporizador = gbt_temporizador_crear(1);
    if(!temporizador){
        printf("%s", gbt_obtener_log());
        return TEMPO_ERR;
    }

    int puntaje = 0;
    uint8_t X_origen = 0;
    uint8_t Y_origen = 0;
    uint8_t fijada = 0;
    eGBT_Tecla tecla;

    while(TRUE)
    {

        fijada = 0;
        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_ESCAPE)
            break;

        // GRAVEDAD POR TIEMPO — va primero
        if(gbt_temporizador_consumir(temporizador)){
            PiezaMoverAbajo(&p);
            if(PiezaDetectarColision(&p, &m)){
                PiezaMoverArriba(&p);
                PiezaVolcar(&m, &p);
                puntaje += EliminarFilasCompletasConPuntaje(&m);
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
                    puntaje+= EliminarFilasCompletasConPuntaje(&m);
                    tipoPieza(&p, generarPiezaAleatoria());
                }
                else
                    puntaje++;
            }
        }


        // DIBUJADO
        gbt_borrar_backbuffer(BRD);
        DibujarTablero(&m, X_origen, Y_origen);
        DibujarPieza(&p);
        DibujarPuntaje(puntaje,320,config.OFFSET_Y,S);
        gbt_volcar_backbuffer();
    }

    gbt_temporizador_destruir(temporizador);
    gbt_destruir_ventana();

    return 0;
}


int8_t IniciarSistema(int argc, char* argv[])
{
    if (argc != 3){
        fprintf(stderr, "ERROR: Faltan argumento/s <ancho> <alto> en %s \n",argv[0]);
        return 1;
    }


    config.ANCHO = atoi(argv[1]), config.ALTO = atoi(argv[2]);
    if(config.ANCHO == 320 && config.ALTO == 200)
    {
        config.ESCALA = 3;
        config.TAM_CELDA = 8;
        config.ESCALA_FUENTE = 1;
    }
    else if (config.ANCHO == 640 && config.ALTO == 480)
    {
        config.ESCALA = 1;
        config.TAM_CELDA = 20;
        config.ESCALA_FUENTE = 3;
    }
    else
        return INIT_ERR;
    config.OFFSET_X = (config.ANCHO - (COL_TABLERO * config.TAM_CELDA)) / 8;
    config.OFFSET_Y = (config.ALTO - (FIL_TABLERO * config.TAM_CELDA)) / 2;

    return 0;
}
