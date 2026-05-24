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


int Jugar(char *nombre);
int IniciarSistema(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    if(IniciarSistema(argc, argv) == INIT_ERR)
        return INIT_ERR;
    semilla();

    int ret = MenuIniciar(config);
    if(ret == SALIR) return 0;

    char* nombre = PantallaIngresoNombre();

    while(1){
        ret = Jugar(nombre);

        if(ret == SALIR)
            break;
        if(ret == MENU_PRINCIPAL){
            ret = MenuIniciar(config);
            if(ret == SALIR) break;
            nombre = PantallaIngresoNombre();
            // si ret == 0 (JUGAR) vuelve a Jugar()
        }
        // si ret == REINICIAR vuelve directo a Jugar()
    }
    free(nombre);
    gbt_destruir_ventana();
    return 0;
}

int IniciarSistema(int argc, char* argv[])
{
    ConfigCargar(CONFIG_FILE);
    config.OFFSET_X = 2 * config.TAM_CELDA;
    config.OFFSET_Y = 4 * config.TAM_CELDA;

    tGBT_ColorRGB paleta[PALETA_MAX_COLORES] = {
        {0x00, 0x00, 0x00}, // N
        {0x00, 0xFF, 0xFF}, // I
        {0x00, 0xAA, 0xAA}, // I_OSCURO
        {0x99, 0xFF, 0xFF}, // I_CLARO
        {0xFF, 0xFF, 0x00}, // O
        {0xAA, 0xAA, 0x00}, // O_OSCURO
        {0xFF, 0xFF, 0x99}, // O_CLARO
        {0x80, 0x00, 0x80}, // T
        {0x55, 0x00, 0x55}, // T_OSCURO
        {0xCC, 0x99, 0xCC}, // T_CLARO
        {0x00, 0xFF, 0x00}, // S
        {0x00, 0xAA, 0x00}, // S_OSCURO
        {0x99, 0xFF, 0x99}, // S_CLARO
        {0xFF, 0x00, 0x00}, // Z
        {0xAA, 0x00, 0x00}, // Z_OSCURO
        {0xFF, 0x99, 0x99}, // Z_CLARO
        {0x00, 0x00, 0xFF}, // J
        {0x00, 0x00, 0xAA}, // J_OSCURO
        {0x99, 0x99, 0xFF}, // J_CLARO
        {0xFF, 0xA5, 0x00}, // L
        {0xAA, 0x6E, 0x00}, // L_OSCURO
        {0xFF, 0xD1, 0x99}, // L_CLARO
        {0x80, 0x80, 0x80}, // BRD
        {0x55, 0x55, 0x55}, // BRD_OSCURO
        {0xCC, 0xCC, 0xCC}, // BRD_CLARO
        {0xFF, 0xFF, 0xFF}, // W
        {0x01, 0x01, 0x01}, // TR
    };
    if(gbt_iniciar() != 0){
        printf("%s", gbt_obtener_log());
        return INIT_ERR;
    }
    if(gbt_crear_ventana(TITULO, config.ANCHO, config.ALTO, config.ESCALA) != 0){
        printf("%s", gbt_obtener_log());
        return INIT_ERR;
    }
    if(gbt_aplicar_paleta(paleta, PALETA_MAX_COLORES, GBT_FORMATO_888) != 0){
        printf("%s", gbt_obtener_log());
        return INIT_ERR;
    }

    config.FIL_TABLERO = 20;
    config.COL_TABLERO = 10;

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
    int lineas = 0;
    int tabla[] = {0, 100, 300, 500, 800};


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
                lineas = EliminarFilasCompletas(&m);
                if(lineas >= 1 && lineas <= 4)
                    puntaje += tabla[lineas] * MultiplicadorPuntos(velActual)* MultiplicadorPuntos(velActual);

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
            sigueApoyada = PiezaDetectarColision(&p, &m);  // faltaba esto
            PiezaMoverArriba(&p);

            if(sigueApoyada && estaEnSuperficie){
                gbt_temporizador_destruir(tempoRetraso);
                tempoRetraso = gbt_temporizador_crear((velActual / 2) / 1000.0);
            } else if(!sigueApoyada){
                estaEnSuperficie = 0;  // faltaba esto
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
                estaEnSuperficie = 0;  // faltaba esto
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
                corriendo = 0;
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

    gbt_temporizador_destruir(temporizador);
    gbt_temporizador_destruir(tempoRetraso);

        if(gameOver){
            for(uint8_t i = 0; i < m.fil; i++)
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

