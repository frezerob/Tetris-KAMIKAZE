/*
Apellido: Freccero, Alejo Benjamin
DNI: 47263450
Usuario: frezerob
Entrega: No
Apellido: Quevedo, Lautaro Joaquin
DNI: 47024422
Usuario: LautaroJQ
Entrega: Si
Apellido: Conti, Dante Javier
DNI: 44510761
Usuario: dantejaconti
Entrega: Si
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


