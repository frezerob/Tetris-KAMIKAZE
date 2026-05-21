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


int Jugar();
int IniciarSistema(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    if(IniciarSistema(argc, argv) == INIT_ERR)
        return INIT_ERR;
    semilla();

    int ret = MenuIniciar(config);
    if(ret == SALIR) return 0;

    while(1){
        ret = Jugar();

        if(ret == SALIR)
            break;
        if(ret == MENU_PRINCIPAL){
            ret = MenuIniciar(config);
            if(ret == SALIR) break;
            // si ret == 0 (JUGAR) vuelve a Jugar()
        }
        // si ret == REINICIAR vuelve directo a Jugar()
    }

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

    return 0;
}


int Jugar()
{
    matrix m;
    PiezaActiva p;

    if(MatrizIniciar(&m, FIL_TABLERO, COL_TABLERO) == INIT_ERR)
        return INIT_ERR;

    int proximas[CANT_PROXIMAS];
    for(int i = 0; i < CANT_PROXIMAS; i++)
        proximas[i] = generarPiezaAleatoria();

    tipoPieza(&p, proximas[0]);
    for(int i = 0; i < CANT_PROXIMAS - 1; i++)
        proximas[i] = proximas[i+1];
    proximas[CANT_PROXIMAS - 1] = generarPiezaAleatoria();

    int velActual = VelocidadSegunDificultad(config.DIFICULTAD);
    int piezasCaidas = 0;
    int puntaje = 0;
    int X_origen = 0;
    int Y_origen = 0;
    int corriendo = 1;
    int fijada = 0;
    int gameOver = 0;
    eGBT_Tecla tecla;

    tGBT_Temporizador* temporizador = gbt_temporizador_crear(velActual / 1000.0);
    if(!temporizador){
        printf("%s", gbt_obtener_log());
        return INIT_ERR;
    }

    while(corriendo)
    {
        fijada = 0;
        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_ESCAPE)
            corriendo = 0;

        // GRAVEDAD POR TIEMPO
        if(gbt_temporizador_consumir(temporizador)){
            PiezaMoverAbajo(&p);
            if(PiezaDetectarColision(&p, &m)){
                PiezaMoverArriba(&p);
                PiezaVolcar(&m, &p);
                puntaje += EliminarFilasCompletasConPuntaje(&m);

                tipoPieza(&p, proximas[0]);
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

                    tipoPieza(&p, proximas[0]);
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

        int xProx = config.OFFSET_X + COL_TABLERO * config.TAM_CELDA + 10;
        int yProx = config.OFFSET_Y + 20;
        for(int i = 0; i < CANT_PROXIMAS; i++)
            DibujarProximaPieza(FORMAS[proximas[i]], xProx, yProx + i * (ORDEN * config.TAM_CELDA + 5));

        gbt_volcar_backbuffer();
    }

    gbt_temporizador_destruir(temporizador);

    if(gameOver)
        return MenuGameOver(puntaje);
    return OK;
}
