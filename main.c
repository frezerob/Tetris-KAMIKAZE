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
#include <string.h>
#include "GBT/gbt.h"
#include "menu.h"
#include "tdatablero.h"
#include "funciones.h"
#include "graficos.h"
#include "config.h"
#include "score.h"
#include "partida.h"

int Jugar(char *nombre);
int JugarDesdeEstado(EstadoPartida* estado);
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
        if(ret == SALIR)
            break;

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
            if(ret == CONTINUAR){
                EstadoPartida estado;
                PartidaCargar(&estado);
                ret = JugarDesdeEstado(&estado);
            } else {
                nombre = PantallaIngresoNombre();
                ret = Jugar(nombre);
            }
        }
        else
            break;
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

    int cheat = (strcmp(nombre, "GOD") == 0) ? 2 : 1;

    if(MatrizIniciar(&m, config.FIL_TABLERO, config.COL_TABLERO) == INIT_ERR)
        return INIT_ERR;

    config.ultimaPosX = config.COL_TABLERO / 2;

    int proximas[CANT_PROXIMAS];
    for(int i = 0; i < CANT_PROXIMAS; i++)
        proximas[i] = generarPiezaAleatoria();

    tipoPieza(&p, proximas[0], &m);
    for(int i = 0; i < CANT_PROXIMAS - 1; i++)
        proximas[i] = proximas[i+1];
    proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

    int velActual    = VelocidadSegunDificultad(config.DIFICULTAD);
    int piezasCaidas = 0;
    int puntaje      = 0;
    int corriendo    = 1;
    int fijada       = 0;
    int gameOver     = 0;
    int lineas       = 0;
    int tabla[]      = {0, 100, 300, 500, 800};
    int estaEnSuperficie = 0;
    int sigueApoyada     = 0;

    tGBT_Temporizador* temporizador = gbt_temporizador_crear(velActual / 1000.0);
    tGBT_Temporizador* tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);

    if(!temporizador || !tempoRetraso){
        printf("%s", gbt_obtener_log());
        return INIT_ERR;
    }

    while(corriendo)
    {
        fijada = 0;
        int seMovio = 0;
        gbt_procesar_entrada();

        if(gbt_tecla_presionada(GBTK_ESCAPE)){
            int opcion = MenuPausa();
            if(opcion == SALIR)
                break;
            if(opcion == GUARDAR_Y_SALIR){
                EstadoPartida estado;
                estado.fil = m.fil;
                estado.col = m.col;
                for(int i = 0; i < m.fil; i++)
                    for(int j = 0; j < m.col; j++)
                        estado.mat[i][j] = m.mat[i][j];
                estado.posX     = p.posX;
                estado.posY     = p.posY;
                estado.rotacion = p.rotacion;
                for(int i = 0; i < 10; i++){
                    if(FORMAS[i] == p.forma){
                        estado.tipoPiezaActual = i;
                        break;
                    }
                }
                for(int i = 0; i < CANT_PROXIMAS; i++)
                    estado.proximas[i] = proximas[i];
                estado.puntaje      = puntaje;
                estado.piezasCaidas = piezasCaidas;
                estado.velActual    = velActual;
                strncpy(estado.nombre, nombre, 4);
                PartidaGuardar(&estado);
                break;
            }
        }

        // BLOQUE 1: LOCK DELAY
        if(estaEnSuperficie){
            if(seMovio){
                gbt_temporizador_destruir(tempoRetraso);
                tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
            }
            if(gbt_temporizador_consumir(tempoRetraso)){
                config.ultimaPosX = p.posX;
                PiezaVolcar(&m, &p);
                lineas = EliminarFilasCompletas(&m);
                if(lineas >= 1 && lineas <= 4)
                    puntaje += tabla[lineas] * MultiplicadorPuntos(velActual) * cheat;

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

        // BLOQUE 2: GRAVEDAD
        if(!fijada && gbt_temporizador_consumir(temporizador)){
            PiezaMoverAbajo(&p);
            if(PiezaDetectarColision(&p, &m)){
                PiezaMoverArriba(&p);
                if(!estaEnSuperficie){
                    estaEnSuperficie = 1;
                    gbt_temporizador_destruir(tempoRetraso);
                    tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
                }
            } else {
                estaEnSuperficie = 0;
            }
        }

        // BLOQUE 3: ENTRADAS
        if(!fijada){
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
                    } else if(!sigueApoyada)
                        estaEnSuperficie = 0;
                }
            }
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
                    } else if(!sigueApoyada)
                        estaEnSuperficie = 0;
                }
            }
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
                    } else if(!sigueApoyada)
                        estaEnSuperficie = 0;
                }
            }
            if(gbt_tecla_presionada(GBTK_q)){
                PiezaRotarIzquierda(&p);
                if(PiezaDetectarColision(&p, &m)){
                    PiezaRotarDerecha(&p);
                } else {
                    seMovio = 1;
                    PiezaMoverAbajo(&p);
                    sigueApoyada = PiezaDetectarColision(&p, &m);
                    PiezaMoverArriba(&p);
                    if(sigueApoyada && estaEnSuperficie){
                        gbt_temporizador_destruir(tempoRetraso);
                        tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
                    } else if(!sigueApoyada)
                        estaEnSuperficie = 0;
                }
            }
            if(gbt_tecla_presionada(GBTK_s)){
                PiezaMoverAbajo(&p);
                if(PiezaDetectarColision(&p, &m)){
                    PiezaMoverArriba(&p);
                    config.ultimaPosX = p.posX;
                    PiezaVolcar(&m, &p);
                    estaEnSuperficie = 0;
                    lineas = EliminarFilasCompletas(&m);
                    if(lineas >= 1 && lineas <= 4)
                        puntaje += tabla[lineas] * MultiplicadorPuntos(velActual) * cheat;

                    tipoPieza(&p, proximas[0], &m);
                    for(int i = 0; i < CANT_PROXIMAS - 1; i++)
                        proximas[i] = proximas[i+1];
                    proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

                    if(PiezaDetectarColision(&p, &m)){
                        corriendo = 0;
                        gameOver  = 1;
                    }

                    piezasCaidas++;
                    if(piezasCaidas % 10 == 0){
                        velActual = RecalcularVelocidad(VelocidadSegunDificultad(config.DIFICULTAD), piezasCaidas);
                        gbt_temporizador_destruir(temporizador);
                        temporizador = gbt_temporizador_crear(velActual / 1000.0);
                    }
                } else {
                    puntaje += MultiplicadorPuntos(velActual) * cheat;
                }
            }
        }

        RenderizarJuego(&p, &m, puntaje, proximas, nombre);
    }

    gbt_temporizador_destruir(temporizador);
    gbt_temporizador_destruir(tempoRetraso);

    if(gameOver){
        for(int i = 0; i < m.fil; i++)
            free(m.mat[i]);
        free(m.mat);
        Score scores[MAX_SCORES];
        int cant = 0;
        ScoresCargar(scores, &cant);
        ScoresAgregar(scores, &cant, nombre, puntaje);
        return MenuGameOver(puntaje);
    }

    return SALIR;
}

int JugarDesdeEstado(EstadoPartida* e)
{
    matrix m;
    PiezaActiva p;

    if(MatrizIniciar(&m, e->fil, e->col) == INIT_ERR)
        return INIT_ERR;

    for(int i = 0; i < e->fil; i++)
        for(int j = 0; j < e->col; j++)
            m.mat[i][j] = e->mat[i][j];

    p.forma    = FORMAS[e->tipoPiezaActual];
    p.posX     = e->posX;
    p.posY     = e->posY;
    p.rotacion = e->rotacion;

    int proximas[CANT_PROXIMAS];
    for(int i = 0; i < CANT_PROXIMAS; i++)
        proximas[i] = e->proximas[i];

    int velActual    = e->velActual;
    int piezasCaidas = e->piezasCaidas;
    int puntaje      = e->puntaje;
    char* nombre     = e->nombre;
    int cheat        = (strcmp(nombre, "GOD") == 0) ? 2 : 1;

    PartidaBorrar();

    int corriendo        = 1;
    int fijada           = 0;
    int gameOver         = 0;
    int lineas           = 0;
    int tabla[]          = {0, 100, 300, 500, 800};
    int estaEnSuperficie = 0;
    int sigueApoyada     = 0;

    tGBT_Temporizador* temporizador = gbt_temporizador_crear(velActual / 1000.0);
    tGBT_Temporizador* tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);

    if(!temporizador || !tempoRetraso)
        return INIT_ERR;

    while(corriendo)
    {
        fijada = 0;
        int seMovio = 0;
        gbt_procesar_entrada();

        if(gbt_tecla_presionada(GBTK_ESCAPE)){
            int opcion = MenuPausa();
            if(opcion == SALIR)
                break;
            if(opcion == GUARDAR_Y_SALIR){
                EstadoPartida estado;
                estado.fil = m.fil;
                estado.col = m.col;
                for(int i = 0; i < m.fil; i++)
                    for(int j = 0; j < m.col; j++)
                        estado.mat[i][j] = m.mat[i][j];
                estado.posX     = p.posX;
                estado.posY     = p.posY;
                estado.rotacion = p.rotacion;
                for(int i = 0; i < 10; i++){
                    if(FORMAS[i] == p.forma){
                        estado.tipoPiezaActual = i;
                        break;
                    }
                }
                for(int i = 0; i < CANT_PROXIMAS; i++)
                    estado.proximas[i] = proximas[i];
                estado.puntaje      = puntaje;
                estado.piezasCaidas = piezasCaidas;
                estado.velActual    = velActual;
                strncpy(estado.nombre, nombre, 4);
                PartidaGuardar(&estado);
                break;
            }
        }

        if(estaEnSuperficie){
            if(seMovio){
                gbt_temporizador_destruir(tempoRetraso);
                tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
            }
            if(gbt_temporizador_consumir(tempoRetraso)){
                config.ultimaPosX = p.posX;
                PiezaVolcar(&m, &p);
                lineas = EliminarFilasCompletas(&m);
                if(lineas >= 1 && lineas <= 4)
                    puntaje += tabla[lineas] * MultiplicadorPuntos(velActual) * cheat;

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

        if(!fijada && gbt_temporizador_consumir(temporizador)){
            PiezaMoverAbajo(&p);
            if(PiezaDetectarColision(&p, &m)){
                PiezaMoverArriba(&p);
                if(!estaEnSuperficie){
                    estaEnSuperficie = 1;
                    gbt_temporizador_destruir(tempoRetraso);
                    tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
                }
            } else {
                estaEnSuperficie = 0;
            }
        }

        if(!fijada){
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
                    } else if(!sigueApoyada)
                        estaEnSuperficie = 0;
                }
            }
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
                    } else if(!sigueApoyada)
                        estaEnSuperficie = 0;
                }
            }
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
                    } else if(!sigueApoyada)
                        estaEnSuperficie = 0;
                }
            }
            if(gbt_tecla_presionada(GBTK_q)){
                PiezaRotarIzquierda(&p);
                if(PiezaDetectarColision(&p, &m)){
                    PiezaRotarDerecha(&p);
                } else {
                    seMovio = 1;
                    PiezaMoverAbajo(&p);
                    sigueApoyada = PiezaDetectarColision(&p, &m);
                    PiezaMoverArriba(&p);
                    if(sigueApoyada && estaEnSuperficie){
                        gbt_temporizador_destruir(tempoRetraso);
                        tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
                    } else if(!sigueApoyada)
                        estaEnSuperficie = 0;
                }
            }
            if(gbt_tecla_presionada(GBTK_s)){
                PiezaMoverAbajo(&p);
                if(PiezaDetectarColision(&p, &m)){
                    PiezaMoverArriba(&p);
                    config.ultimaPosX = p.posX;
                    PiezaVolcar(&m, &p);
                    estaEnSuperficie = 0;
                    lineas = EliminarFilasCompletas(&m);
                    if(lineas >= 1 && lineas <= 4)
                        puntaje += tabla[lineas] * MultiplicadorPuntos(velActual) * cheat;

                    tipoPieza(&p, proximas[0], &m);
                    for(int i = 0; i < CANT_PROXIMAS - 1; i++)
                        proximas[i] = proximas[i+1];
                    proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

                    if(PiezaDetectarColision(&p, &m)){
                        corriendo = 0;
                        gameOver  = 1;
                    }

                    piezasCaidas++;
                    if(piezasCaidas % 10 == 0){
                        velActual = RecalcularVelocidad(VelocidadSegunDificultad(config.DIFICULTAD), piezasCaidas);
                        gbt_temporizador_destruir(temporizador);
                        temporizador = gbt_temporizador_crear(velActual / 1000.0);
                    }
                } else {
                    puntaje += MultiplicadorPuntos(velActual) * cheat;
                }
            }
        }

        RenderizarJuego(&p, &m, puntaje, proximas, nombre);
    }

    gbt_temporizador_destruir(temporizador);
    gbt_temporizador_destruir(tempoRetraso);

    if(gameOver){
        for(int i = 0; i < m.fil; i++)
            free(m.mat[i]);
        free(m.mat);
        Score scores[MAX_SCORES];
        int cant = 0;
        ScoresCargar(scores, &cant);
        ScoresAgregar(scores, &cant, nombre, puntaje);
        return MenuGameOver(puntaje);
    }

    return SALIR;
}
