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
#include "config.h"



int8_t IniciarSistema(int argc, char* argv[]);



int main(int argc, char* argv[])
{
    IniciarSistema(argc, argv);
    semilla();

    matrix m;
    PiezaActiva p;

    int retMenu = MenuIniciar(config);
    if(retMenu == INIT_ERR) return INIT_ERR;
    if(retMenu == SALIR)    return 0;
    if(MatrizIniciar(&m, FIL_TABLERO, COL_TABLERO) == INIT_ERR)
        return INIT_ERR;

    // Inicializamos el array de proximas piezas
    int proximas[CANT_PROXIMAS];
    for(int i = 0; i < CANT_PROXIMAS; i++)
        proximas[i] = generarPiezaAleatoria();

    // La pieza actual es la primera del array
    tipoPieza(&p, proximas[0]);
    for(int i = 0; i < CANT_PROXIMAS - 1; i++)
        proximas[i] = proximas[i+1];
    proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

    // Velocidad
    int velActual = VelocidadSegunDificultad(config.DIFICULTAD);
    int piezasCaidas = 0;

    tGBT_Temporizador* temporizador = gbt_temporizador_crear(velActual / 1000.0);
    if(!temporizador){
        printf("%s", gbt_obtener_log());
        return TEMPO_ERR;
    }

    int puntaje = 0;
    int X_origen = 0;
    int Y_origen = 0;
    int fijada = 0;
    eGBT_Tecla tecla;

    while(TRUE)
    {
        fijada = 0;
        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_ESCAPE)
            break;

        // GRAVEDAD POR TIEMPO
        if(gbt_temporizador_consumir(temporizador)){
            PiezaMoverAbajo(&p);
            if(PiezaDetectarColision(&p, &m)){
                PiezaMoverArriba(&p);
                PiezaVolcar(&m, &p);
                puntaje += EliminarFilasCompletasConPuntaje(&m);

                // Rotamos el array de proximas
                tipoPieza(&p, proximas[0]);
                for(int i = 0; i < CANT_PROXIMAS - 1; i++)
                    proximas[i] = proximas[i+1];
                proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

                // Velocidad creciente cada 10 piezas
                piezasCaidas++;
                if(piezasCaidas % 10 == 0){
                    velActual = RecalcularVelocidad(VelocidadSegunDificultad(config.DIFICULTAD), piezasCaidas);
                    gbt_temporizador_destruir(temporizador);
                    temporizador = gbt_temporizador_crear(velActual / 1000.0);
                    printf("%d",velActual);
                }

                fijada = 1;
            }
        }

        // TECLAS
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
                    puntaje += EliminarFilasCompletasConPuntaje(&m);

                    // Rotamos el array de proximas
                    tipoPieza(&p, proximas[0]);
                    for(int i = 0; i < CANT_PROXIMAS - 1; i++)
                        proximas[i] = proximas[i+1];
                    proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

                    piezasCaidas++;
                    if(piezasCaidas % 10 == 0){
                        velActual = RecalcularVelocidad(VelocidadSegunDificultad(config.DIFICULTAD), piezasCaidas);
                        gbt_temporizador_destruir(temporizador);
                        temporizador = gbt_temporizador_crear(velActual / 1000.0);
                        printf("%d",velActual);
                    }
                }
                else
                    puntaje++;
            }
        }

        // DIBUJADO
        gbt_borrar_backbuffer(N);
        DibujarFondo();

        DibujarTablero(&m, X_origen, Y_origen);
        DibujarPieza(&p);
        DibujarRectangulo(config.OFFSET_X + (COL_TABLERO+1) * config.TAM_CELDA ,config.OFFSET_Y,15,20,N,PLANO);
        DibujarRectangulo(config.OFFSET_X,10,26,2,N,PLANO);
        DibujarTexto("TETRIS KAMIKAZE",config.OFFSET_X,15,S);
        DibujarTextoCentrado("PUNTAJE",config.OFFSET_Y + 7,T,-config.OFFSET_X -5);
        DibujarPuntaje(puntaje, config.OFFSET_X + COL_TABLERO * config.TAM_CELDA + CalcularAnchoTexto("PUNTAJE") + 15, config.OFFSET_Y + 7, T);

        // Dibujamos las proximas piezas a la derecha del tablero
        int xProx = config.OFFSET_X + COL_TABLERO * config.TAM_CELDA + 10;
        int yProx = config.OFFSET_Y + 20;
        for(int i = 0; i < CANT_PROXIMAS; i++)
            DibujarProximaPieza(FORMAS[proximas[i]], xProx, yProx + i * (ORDEN * config.TAM_CELDA + 5));

        gbt_volcar_backbuffer();
    }

    gbt_temporizador_destruir(temporizador);
    gbt_destruir_ventana();

    return 0;
}


int8_t IniciarSistema(int argc, char* argv[])
{
     ConfigCargar(CONFIG_FILE);
    // OFFSET se calcula después de tener TAM_CELDA
    //config.OFFSET_X = (config.ANCHO - (COL_TABLERO * config.TAM_CELDA)) / 8;
    config.OFFSET_X = 2 * config.TAM_CELDA;
    config.OFFSET_Y = 4 * config.TAM_CELDA;
    //config.OFFSET_Y = (config.ALTO  - (FIL_TABLERO * config.TAM_CELDA)) / 2 + 10;

    return 0;
}
