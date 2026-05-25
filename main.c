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
#include "score.h"
#include "partida.h"
#include "core.h"
#include "juego.h"


int IniciarSistema(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    if(IniciarSistema(argc, argv) == INIT_ERR)
        return INIT_ERR;
    semilla();

    int ret = MenuIniciar(config);
    if(ret == SALIR) return 0;

    char* nombre = NULL;

    if(ret == CONTINUAR){
        EstadoPartida estado;
        PartidaCargar(&estado);
        ret = JugarDesdeEstado(&estado);
    } else {
        nombre = PantallaIngresoNombre();
    }

    while(1){
        if(ret == SALIR) break;

        if(ret == 0 || ret == REINICIAR){
            if(nombre == NULL)
                nombre = PantallaIngresoNombre();
            ret = Jugar(nombre);
        }
        else if(ret == CONTINUAR){
            EstadoPartida estado;
            PartidaCargar(&estado);
            ret = JugarDesdeEstado(&estado);
        }
        else if(ret == MENU_PRINCIPAL){
            if(nombre) free(nombre);
            nombre = NULL;
            ret = MenuIniciar(config);
            if(ret == SALIR) break;
            free(nombre);
            if(ret == CONTINUAR){
                EstadoPartida estado;
                PartidaCargar(&estado);
                ret = JugarDesdeEstado(&estado);
            } else {
                nombre = PantallaIngresoNombre();
                ret = Jugar(nombre);
            }
        }
        else break;
    }

    if(nombre) free(nombre);
    gbt_destruir_ventana();
    return 0;
}

int IniciarSistema(int argc, char* argv[])
{
    ConfigCargar(CONFIG_FILE);
    config.OFFSET_X = 2 * config.TAM_CELDA;
    config.OFFSET_Y = 4 * config.TAM_CELDA;

    if(gbt_iniciar() != 0){
        printf("%s", gbt_obtener_log());
        return INIT_ERR;
    }
    if(gbt_crear_ventana(TITULO, config.ANCHO, config.ALTO, config.ESCALA) != 0){
        printf("%s", gbt_obtener_log());
        return INIT_ERR;
    }

    AplicarPaleta(config.PALETA);
    return 0;
}


int Jugar(char *nombre)
{
    matrix m;
    PiezaActiva p;

    uint8_t cheat = NombreEsGod(nombre) ? 2 : 1;

    if(MatrizIniciar(&m, config.FIL_TABLERO, config.COL_TABLERO) == INIT_ERR)
        return INIT_ERR;

    config.ultimaPosX = config.COL_TABLERO/2;

    int proximas[CANT_PROXIMAS];
    for(int i = 0; i < CANT_PROXIMAS; i++)
        proximas[i] = generarPiezaAleatoria();

    tipoPieza(&p, proximas[0], &m);
    for(int i = 0; i < CANT_PROXIMAS - 1; i++)
        proximas[i] = proximas[i+1];
    proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

    uint16_t velActual = VelocidadSegunDificultad(config.DIFICULTAD) * cheat;
    int piezasCaidas = 0;
    int puntaje =  0;
    uint8_t corriendo = 1;
    uint8_t fijada = 0;
    int gameOver = 0;


    // INICIALIZACIÓN DE TEMPORIZADORES (Siempre dividiendo por 1000.0 para pasar a segundos)
    tGBT_Temporizador* temporizador = gbt_temporizador_crear(velActual / 1000.0);
    tGBT_Temporizador* tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);

    // Variables de retraso de fijación
    uint8_t estaEnSuperficie = 0;
    uint8_t sigueApoyada = 0;

    if(!temporizador || !tempoRetraso){
        printf("%s", gbt_obtener_log());
        return INIT_ERR;
    }

    while(corriendo)
    {
        fijada = 0;

        uint8_t seMovio = 0;
        gbt_procesar_entrada();

        // CONTROL DE SALIDA/PAUSA
        if(gbt_tecla_presionada(GBTK_ESCAPE)){
            int8_t opcion = MenuPausa();
            if(opcion == SALIR)
                break;
        }

        // BLOQUE 1: PROCESAMIENTO DEL LOCK DELAY Si expiró el tiempo, se fija
        if(estaEnSuperficie){
                if(seMovio){
        // se movió este frame, reseteamos el timer
        gbt_temporizador_destruir(tempoRetraso);
        tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
                            }
            if(gbt_temporizador_consumir(tempoRetraso)){
                config.ultimaPosX = p.posX;
                PiezaVolcar(&m, &p);
                puntaje += EliminarFilasCompletasConPuntaje(&m) * MultiplicadorPuntos(velActual);

                tipoPieza(&p, proximas[0], &m);
                for(int i = 0; i < CANT_PROXIMAS - 1; i++)
                    proximas[i] = proximas[i+1];
                proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

                if(PiezaDetectarColision(&p, &m)){
                    gameOver = 1;
                    break;
                }

                piezasCaidas++;
                if(piezasCaidas % 10 == 0){
                    velActual = RecalcularVelocidad(VelocidadSegunDificultad(config.DIFICULTAD), piezasCaidas);
                    gbt_temporizador_destruir(temporizador);
                    temporizador = gbt_temporizador_crear(velActual / 1000.0);
                }

                estaEnSuperficie = 0;
                fijada = 1;
            }
        }

        // BLOQUE 2: GRAVEDAD POR TIEMPO (Solo detecta la superficie y activa el tiempo de fijacion)
        if(!fijada && gbt_temporizador_consumir(temporizador)){
            PiezaMoverAbajo(&p);
            if(PiezaDetectarColision(&p, &m)){
                PiezaMoverArriba(&p); // Deshacer movimiento inválido

                if(!estaEnSuperficie){
                    estaEnSuperficie = 1;
                    // Resetear el reloj de gracia recreándolo
                    gbt_temporizador_destruir(tempoRetraso);
                    tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
                }
            } else {
                // Si la pieza pudo bajar con éxito, ya no está en una superficie
                estaEnSuperficie = 0;
            }
        }

        // BLOQUE 3: CONTROL DE ENTRADAS DEL JUGADOR
       if(!fijada){
    // MOVER IZQUIERDA
    if(gbt_tecla_presionada(GBTK_a)){
        PiezaMoverIzq(&p);
        if(PiezaDetectarColision(&p, &m)){
            PiezaMoverDer(&p);
        } else {
            seMovio = 1;
            PiezaMoverAbajo(&p);
            sigueApoyada = PiezaDetectarColision(&p, &m);
            PiezaMoverArriba(&p);

            if(sigueApoyada && estaEnSuperficie){
                gbt_temporizador_destruir(tempoRetraso);
                tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
            } else if(!sigueApoyada){
                estaEnSuperficie = 0;  // se movio y ya no toca nada
            }
        }
    }

    // MOVER DERECHA
    if(gbt_tecla_presionada(GBTK_d)){
        PiezaMoverDer(&p);
        if(PiezaDetectarColision(&p, &m)){
            PiezaMoverIzq(&p);
        } else {
            seMovio = 1;
            PiezaMoverAbajo(&p);
            sigueApoyada = PiezaDetectarColision(&p, &m);
            PiezaMoverArriba(&p);

            if(sigueApoyada && estaEnSuperficie){
                gbt_temporizador_destruir(tempoRetraso);
                tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
            } else if(!sigueApoyada){
                estaEnSuperficie = 0;
            }
        }
    }

    // ROTAR DERECHA
    if(gbt_tecla_presionada(GBTK_w)){
        PiezaRotarDerecha(&p);
        if(PiezaDetectarColision(&p, &m)){
            PiezaRotarIzquierda(&p);
        } else {
            seMovio = 1;
            PiezaMoverAbajo(&p);
            sigueApoyada = PiezaDetectarColision(&p, &m);
            PiezaMoverArriba(&p);

            if(sigueApoyada && estaEnSuperficie){
                gbt_temporizador_destruir(tempoRetraso);
                tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
            } else if(!sigueApoyada){
                estaEnSuperficie = 0;
            }
        }
    }

    if(gbt_tecla_presionada(GBTK_s)){
        PiezaMoverAbajo(&p);
        if(PiezaDetectarColision(&p, &m)){
            PiezaMoverArriba(&p);
            config.ultimaPosX = p.posX;
            // Volcado instantáneo por empuje manual
            PiezaVolcar(&m, &p);
            estaEnSuperficie = 0;
            puntaje += EliminarFilasCompletasConPuntaje(&m) * MultiplicadorPuntos(velActual);

            tipoPieza(&p, proximas[0], &m);
            for(uint8_t i = 0; i < CANT_PROXIMAS - 1; i++)
                proximas[i] = proximas[i+1];
            proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

            if(PiezaDetectarColision(&p, &m)){
                gameOver = 1;
            }

            piezasCaidas++;
            if(piezasCaidas % 10 == 0){
                velActual = RecalcularVelocidad(VelocidadSegunDificultad(config.DIFICULTAD), piezasCaidas);
                gbt_temporizador_destruir(temporizador);
                temporizador = gbt_temporizador_crear(velActual / 1000.0);
            }
        }
        else {
            puntaje++;
        }
            }
        }
        RenderizarJuego(&p, &m, puntaje, proximas, nombre);
    }

    // EVITAMOS MEMORY LEAK Y DESTRUIMOS TEMPORIZADORES
    MatrizBorrar(&m);
    gbt_temporizador_destruir(temporizador);
    gbt_temporizador_destruir(tempoRetraso);

        if(gameOver){
            Score scores[MAX_SCORES];
            int cant = 0;
            ScoresCargar(scores, &cant);
            ScoresAgregar(scores, &cant, nombre, puntaje);
            return MenuGameOver(puntaje);
        }

        return SALIR;
}


/*    // BAJAR MANUAL
    if(gbt_tecla_presionada(GBTK_s)){
        PiezaMoverAbajo(&p);
        if(PiezaDetectarColision(&p, &m)){
            PiezaMoverArriba(&p);
            PiezaVolcar(&m, &p);
            estaEnSuperficie = 0;
            lineas = EliminarFilasCompletas(&m);
            if(lineas >= 1 && lineas <= 4)
                puntaje += tabla[lineas] * MultiplicadorPuntos(velActual);

            tipoPieza(&p, proximas[0], &m);
            for(int i = 0; i < CANT_PROXIMAS - 1; i++)
                proximas[i] = proximas[i+1];
            proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

            if(PiezaDetectarColision(&p, &m)){
                corriendo = 0;
                gameOver = 1;
            }

            piezasCaidas++;
            if(piezasCaidas % 10 == 0){
                velActual = RecalcularVelocidad(VelocidadSegunDificultad(config.DIFICULTAD), piezasCaidas);
                gbt_temporizador_destruir(temporizador);
                temporizador = gbt_temporizador_crear(velActual / 1000.0);
            }
        } else {
            puntaje++;
        }
    }
*/

